#!/usr/bin/env python
# Matthew Bolda

import sys
from BitVector import *
from rsa import *



def solve_pRoot(p, x):  # O(lgn) solution

    # Upper bound u is set to as follows:
    # We start with the 2**0 and keep increasing the power so that u is 2**1, 2**2, ...
    # Until we hit a u such that u**p is > x
    u = 1
    while u ** p <= x: u *= 2

    # Lower bound set to half of upper bound
    l = u // 2

    # Keep the search going until upper u becomes less than lower l
    while l < u:
        mid = (l + u) // 2
        mid_pth = mid ** p
        if l < mid and mid_pth < x:
            l = mid
        elif u > mid and mid_pth > x:
            u = mid
        else:
            # Found perfect pth root.
            return mid
    return mid + 1


# basically generate 3 sets of 2 prime numbers, encrpyt using them and then write to respective files
def solveRSA(messagefile, enc1, enc2, enc3, outputfile, e):
    # get 3 sets of 2 prime numbers
    p1, q1 = genprimes(e)
    p2, q2 = genprimes(e)
    p3, q3 = genprimes(e)

    # fill in the enc files
    RSAencrypt(messagefile, p1, q1, enc1, e)
    RSAencrypt(messagefile, p2, q2, enc2, e)
    RSAencrypt(messagefile, p3, q3, enc3, e)

    # get the public key for each enc file
    n1 = p1 * q1
    n2 = p2 * q2
    n3 = p3 * q3

    # write n values to n_1_2_3 file with \n in between
    output_file = open(outputfile, 'w')
    output_file.write(str(n1))
    output_file.write("\n")
    output_file.write(str(n2))
    output_file.write("\n")
    output_file.write(str(n3))
    output_file.close()

    return


# read all three n values in a single function to reduce cluster
def read_n(nfile):
    n_file = open(nfile, 'r')
    n1 = int(n_file.readline())
    n2 = int(n_file.readline())
    n3 = int(n_file.readline())
    n_file.close()
    return n1, n2, n3


# Do all three BitVector conversions in a single function to reduce cluster
def create_n_bv(n1, n2, n3):
    n1_bv = BitVector(intVal=n1)
    n2_bv = BitVector(intVal=n2)
    n3_bv = BitVector(intVal=n3)
    return n1_bv, n2_bv, n3_bv


# my implementation of CRT
def Chinese_remainder_theorem(Cipher1, Cipher2, Cipher3, Inverse1, Inverse2, Inverse3, N, M1, M2, M3):
    # Mcubed = Cipheri * mi * coeffi + ...
    Mcubed = Cipher1 * Inverse1 * M1
    Mcubed += Cipher2 * Inverse2 * M2
    Mcubed += Cipher3 * Inverse3 * M3
    Mcubed = Mcubed % N

    return Mcubed


# make getting hexstring 1 line function call
def returnhexstring(file):
    file_in = open(file, 'r')
    entire_file = file_in.read()
    hexstring = BitVector(hexstring=entire_file)
    file_in.close()
    return hexstring

# Do the hard stuff, or fix RSAdecrypt to help
def crackRSA(enc1, enc2, enc3, n123file, outputfile, e):
    output_file = open(outputfile, 'w')

    # get ns
    n1, n2, n3 = read_n(n123file)
    n1_bv, n2_bv, n3_bv = create_n_bv(n1, n2, n3)
    N = n1 * n2 * n3

    # get Ms and inverses
    M1 = N // n1
    M2 = N // n2
    M3 = N // n3

    # get M inverses
    M1_inverse = int(BitVector(intVal=M1).multiplicative_inverse(BitVector(intVal=n1)))
    M2_inverse = int(BitVector(intVal=M2).multiplicative_inverse(BitVector(intVal=n2)))
    M3_inverse = int(BitVector(intVal=M3).multiplicative_inverse(BitVector(intVal=n3)))

    # THE FILES ARE IN HEX SO CANT JUST DO FILENAME BADDD
    # enc1_bv = BitVector(filename=enc1)
    # enc2_bv = BitVector(filename=enc2)
    # enc3_bv = BitVector(filename=enc3)

    # get hexstring from each file
    enc1_hex = returnhexstring(enc1)
    enc2_hex = returnhexstring(enc2)
    enc3_hex = returnhexstring(enc3)

    # all sizes should be the same, get length of 1 for end condition
    i = 0
    length = enc1_hex.length()
    end_condition = i < length

    while end_condition:
        # get a 256-bit block from each file
        hexC1 = enc1_hex[i:i + 256]
        hexC2 = enc2_hex[i:i + 256]
        hexC3 = enc3_hex[i:i + 256]

        # convert hex string to int
        C1 = int(hexC1)
        C2 = int(hexC2)
        C3 = int(hexC3)

        # Use Chinese Remainder Theorem to get x or M^3
        x = Chinese_remainder_theorem(C1, C2, C3, M1_inverse, M2_inverse, M3_inverse, N, M1, M2, M3)
        x = x % N
        # x = (C1)(M1)(M1_inverse) + ...

        # x = M^3, solve for cube root for original message
        M = solve_pRoot(3, x)
        M = BitVector(intVal=M, size=256)

        # update end_condition
        i = i + 256
        end_condition = i < length

        # convert to 128 by splitting into two parts, 128 padding and 128 message, write to file
        padding, M = M.divide_into_two()

        # this statement makes sure we do not output unset bits
        # fix: need to find where 00001101 is coming from, its showing as second to last bit-code
        if end_condition == False:
            for j in range(0, 128, 8):
                if int(M[j:j + 8]) != 0:
                    test = M[j:j + 8]
                    Message_part = M[j:j + 8]
                    message_ascii = Message_part.get_bitvector_in_ascii()
                    output_file.write(message_ascii)
        # this statement is for all but the last chunk
        else:
            message_ascii = M.get_bitvector_in_ascii()
            output_file.write(message_ascii)

    return


# argv[1]   argv[2]       argv[3]   argv[4]     argv[5]       argv[6]
# -e        message.txt   enc1.txt  enc2.txt    enc3.txt      n_1_2_3.txt
# -c        enc1.txt      enc2.txt  enc3.txt    n_1_2_3.txt   cracked.txt

if __name__ == "__main__":
    e = 3
    flag = sys.argv[1]

    # encrypting
    if flag == '-e':
        message = sys.argv[2]
        enc1 = sys.argv[3]
        enc2 = sys.argv[4]
        enc3 = sys.argv[5]
        n_1_2_3 = sys.argv[6]
        solveRSA(message, enc1, enc2, enc3, n_1_2_3, e)

    # decrypting, or "cracking"
    if flag == '-c':
        enc1 = sys.argv[2]
        enc2 = sys.argv[3]
        enc3 = sys.argv[4]
        n_1_2_3 = sys.argv[5]
        cracked = sys.argv[6]
        crackRSA(enc1, enc2, enc3, n_1_2_3, cracked, e)
