#!/usr/bin/env python

#   Name: Matthew Bolda
#   login: mbolda
#   date: 1/20/2020
#   key as string:  br
#   key as int:     25202
#   key_bv:         0110001001110010
#   message:
#   It is my belief that nearly any invented quotation, played with confidence, stands a good chance to deceive.
#
#
#
# - Mark Twain
# Time: anywhere from 250-550 seconds on my laptop

from BitVector import *


def cryptBreak(ciphertextFile, key_bv):
    PassPhrase = "Hopes and dreams of a million years"  
    BLOCKSIZE = 16  
    numbytes = BLOCKSIZE // 8  

    # Reduce the passphrase to a bit array of size BLOCKSIZE:
    bv_iv = BitVector(bitlist=[0] * BLOCKSIZE)  
    for i in range(0, len(PassPhrase) // numbytes):  
        textstr = PassPhrase[i * numbytes:(i + 1) * numbytes]  
        bv_iv ^= BitVector(textstring=textstr)  

    # Create a bitvector from the ciphertext hex string:
    FILEIN = open(ciphertextFile, 'r')  
    encrypted_bv = BitVector(hexstring=FILEIN.read())  

    # Create a bitvector for storing the decrypted plaintext bit array:
    msg_decrypted_bv = BitVector(size=0)  
    # Carry out differential XORing of bit blocks and decryption:
    previous_decrypted_block = bv_iv  

    for i in range(0, len(encrypted_bv) // BLOCKSIZE):  
        bv = encrypted_bv[i * BLOCKSIZE:(i + 1) * BLOCKSIZE]  
        temp = bv.deep_copy()  
        bv ^= previous_decrypted_block  
        previous_decrypted_block = temp  
        bv ^= key_bv  
        msg_decrypted_bv += bv  

    outputtext = msg_decrypted_bv.get_text_from_bitvector()  
    return msg_decrypted_bv.get_text_from_bitvector()


# Write plaintext to the output file:
if __name__ == '__main__':
    for i in range(0, pow(2, 16)):
        key_bv = BitVector(intVal=i, size=16)
        plaintext = cryptBreak('encrypted.txt', key_bv)
        if "Mark Twain" in plaintext:
            FILEOUT = open(sys.argv[2], 'w')
            #FILEOUT.write("Key: " + key_bv.get_text_from_bitvector() + "\n")
            FILEOUT.write(plaintext)
            FILEOUT.close()
            break
