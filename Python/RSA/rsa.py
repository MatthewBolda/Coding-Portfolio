#!/usr/bin/env python
# Matthew Bolda

import sys
from BitVector import *
import sys
import random

# This is code from lecture
class PrimeGenerator(object):

    def __init__(self, **kwargs):
        bits = debug = None
        if 'bits' in kwargs:     bits = kwargs.pop('bits')
        if 'debug' in kwargs:     debug = kwargs.pop('debug')
        self.bits = bits
        self.debug = debug
        self._largest = (1 << bits) - 1

    def set_initial_candidate(self):
        candidate = random.getrandbits(self.bits)
        if candidate & 1 == 0: candidate += 1
        candidate |= (1 << self.bits - 1)
        candidate |= (2 << self.bits - 3)
        self.candidate = candidate

    def set_probes(self):
        self.probes = [2, 3, 5, 7, 11, 13, 17]


    def test_candidate_for_prime(self):
        p = self.candidate
        if p == 1: return 0
        if p in self.probes:
            self.probability_of_prime = 1
            return 1
        if any([p % a == 0 for a in self.probes]): return 0
        k, q = 0, self.candidate - 1
        while not q & 1:
            q >>= 1
            k += 1
        if self.debug: print("q = %d  k = %d" % (q, k))
        for a in self.probes:
            a_raised_to_q = pow(a, q, p)
            if a_raised_to_q == 1 or a_raised_to_q == p - 1: continue
            a_raised_to_jq = a_raised_to_q
            primeflag = 0
            for j in range(k - 1):
                a_raised_to_jq = pow(a_raised_to_jq, 2, p)
                if a_raised_to_jq == p - 1:
                    primeflag = 1
                    break
            if not primeflag: return 0
        self.probability_of_prime = 1 - 1.0 / (4 ** len(self.probes))
        return self.probability_of_prime

    def findPrime(self):
        self.set_initial_candidate()
        if self.debug:  print("    candidate is: %d" % self.candidate)
        self.set_probes()
        if self.debug:  print("    The probes are: %s" % str(self.probes))
        max_reached = 0
        while 1:
            if self.test_candidate_for_prime():
                if self.debug:
                    print("Prime number: %d with probability %f\n" %
                          (self.candidate, self.probability_of_prime))
                break
            else:
                if max_reached:
                    self.candidate -= 2
                elif self.candidate >= self._largest - 2:
                    max_reached = 1
                    self.candidate -= 2
                else:
                    self.candidate += 2
                if self.debug:
                    print("    candidate is: %d" % self.candidate)
        return self.candidate


# This function was taken from Lecture code
def gcd(a, b):
    while b:
        a, b = b, a % b
    return a


# function to test needed conditions
def testconditions(p, q, e):
    # test if two left-most bits are set
    test_bits = BitVector(bitstring='11')
    p_bits = BitVector(intVal=p)
    p_last_bits = p_bits[0:2]
    q_bits = BitVector(intVal=q)
    q_last_bits = q_bits[0:2]
    if p_last_bits != test_bits or q_last_bits != test_bits:
        return False

    # test if p and q are equal
    if p == q:
        return False

    # (p-1) and (q-1) should be co-prime to e
    p_coprime = gcd((p - 1), e)
    q_coprime = gcd((q - 1), e)
    if p_coprime != 1 and q_coprime != 1:
        return False
    return True


# this function uses the lecture code of PrimeGenerator.py and tests if passes conditions
def genprimes(e):
    p = 0
    q = 0
    valid_primes = False
    while not valid_primes:
        primeGen = PrimeGenerator(bits=128)
        p = primeGen.findPrime()
        q = primeGen.findPrime()
        valid_primes = testconditions(p, q, e)
    return p, q


# blocks are not 256 bits but they need to be, first pad right if <128, then pad left to make 256
def fixto256(block_bv):
    # make sure its 128 bits, if not pad from right
    length = block_bv.length()
    if length < 128:
        block_bv.pad_from_right(128 - length)
    # for our assignment it should be 256 so pad left from there
    block_bv.pad_from_left(128)
    return block_bv


# using ciphertext = M^e mod n to encrypt
def RSAencrypt(inputfile, p, q, outputfile, e):
    # open the output to write and get bitvector of incoming message
    outputfile = open(outputfile, 'w')
    message_bv = BitVector(filename=inputfile)

    # if for some reason e is type BitVector or something else, convert it.
    if type(e) != int:
        e = int(e)
    n = p * q

    while message_bv.more_to_read:
        # read 128 bits at a time, use helper function to convert to 256
        block_bv = message_bv.read_bits_from_file(128)
        block_bv = fixto256(block_bv)

        # ciphertext = M^e mod n
        C = pow(int(block_bv), e, n)
        encrypted = BitVector(intVal=C, size=256)

        # convert to hex
        hex_encrypted = encrypted.get_bitvector_in_hex()
        outputfile.write(hex_encrypted)

    outputfile.close()
    return


# function to find decryption key
def calculateD(e, p, q, n):
    totient = (p - 1) * (q - 1)
    totient = BitVector(intVal=totient)
    if type(e) != BitVector:
        e = BitVector(intVal=e)
    e_inverse = e.multiplicative_inverse(modulus=totient)
    d = int(e_inverse)

    return d


# using Message = C^d mod n to decrypt
def RSAdecrypt(inputfile, p, q, outputfile, e):
    outputfile = open(outputfile, 'w')
    # make sure that if e is not passed as int it is converted
    if type(e) != int:
        e = int(e)
    n = p * q

    d = calculateD(e, p, q, n)

    # get the whole hexstring
    inputfile = open(inputfile, 'r')
    encrypted = inputfile.read()
    encrypted_bv = BitVector(hexstring=encrypted)
    encrypted_length = encrypted_bv.length()

    i = 0
    while i < encrypted_length:
        # split the encrypted file into blocks of 256
        encrypted_block = encrypted_bv[i: i + 256]
        i = i + 256

        # Message = C^d mod n
        M = pow(int(encrypted_block), d, n)
        message = BitVector(intVal=M, size=256)

        # split the message in two to remove the 128 bits of padding, trash is padding
        trash, message = message.divide_into_two()
        # this statement makes sure we do not output unset bits
        if not (i < encrypted_length):
            for j in range(0, 128, 8):
                if int(message[j:j + 8]) != 0:
                    Message_part = message[j:j + 8]
                    message_ascii = Message_part.get_bitvector_in_ascii()
                    outputfile.write(message_ascii)
        # this statement is for all but the last chunk
        else:
            message = message.get_text_from_bitvector()
            outputfile.write(message)

    outputfile.close()
    return


# argv[1]   argv[2]       argv[3]   argv[4]   argv[5]
# -g        p.txt         q.txt
# -e        message.txt   p.txt     q.txt     encrypted.txt
# -d        encrypted.txt p.txt     q.txt     decrypted.txt
if __name__ == "__main__":
    e = 65537

    flag = sys.argv[1]

    if flag == '-g':
        p, q = genprimes(e)
        pfile = sys.argv[2]
        qfile = sys.argv[3]

        p_file_write = open(pfile, 'w')
        q_file_write = open(qfile, 'w')
        p_file_write.write(str(p))
        q_file_write.write(str(q))
        p_file_write.close()
        q_file_write.close()

    if flag == '-e' or flag == '-d':
        inputfile = sys.argv[2]
        # '''
        pfile = sys.argv[3]
        pfile_read = open(pfile, 'r')
        p_char = pfile_read.read()
        p = int(p_char)
        pfile_read.close()
        qfile = sys.argv[4]
        qfile_read = open(qfile, 'r')
        q_char = qfile_read.read()
        q = int(q_char)
        pfile_read.close()
        # '''
        outputfile = sys.argv[5]
        if flag == '-e':
            RSAencrypt(inputfile, p, q, outputfile, e)
        if flag == '-d':
            RSAdecrypt(inputfile, p, q, outputfile, e)
