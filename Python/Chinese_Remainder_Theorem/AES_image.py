#!/usr/bin/env python3
# Matthew Bolda
# takes about 9 1/2 mins to run but works

import sys
from BitVector import *

AES_modulus = BitVector(bitstring='100011011')
subBytesTable = []  # for encryption
invSubBytesTable = []  # for decryption


def genTables():
    c = BitVector(bitstring='01100011')
    d = BitVector(bitstring='00000101')
    for i in range(0, 256):
        # For the encryption SBox
        a = BitVector(intVal=i, size=8).gf_MI(AES_modulus, 8) if i != 0 else BitVector(intVal=0)
        # For bit scrambling for the encryption SBox entries:
        a1, a2, a3, a4 = [a.deep_copy() for x in range(4)]
        a ^= (a1 >> 4) ^ (a2 >> 5) ^ (a3 >> 6) ^ (a4 >> 7) ^ c
        subBytesTable.append(int(a))
        # For the decryption Sbox:
        b = BitVector(intVal=i, size=8)
        # For bit scrambling for the decryption SBox entries:
        b1, b2, b3 = [b.deep_copy() for x in range(3)]
        b = (b1 >> 2) ^ (b2 >> 5) ^ (b3 >> 7) ^ d
        check = b.gf_MI(AES_modulus, 8)
        b = check if isinstance(check, BitVector) else 0
        invSubBytesTable.append(int(b))


def gen_key_schedule_256(key_bv):
    byte_sub_table = gen_subbytes_table()
    key_words = [None for i in range(60)]
    round_constant = BitVector(intVal=0x01, size=8)
    for i in range(8):
        key_words[i] = key_bv[i * 32: i * 32 + 32]
    for i in range(8, 60):
        if i % 8 == 0:
            kwd, round_constant = gee(key_words[i - 1], round_constant, byte_sub_table)
            key_words[i] = key_words[i - 8] ^ kwd
        elif (i - (i // 8) * 8) < 4:
            key_words[i] = key_words[i - 8] ^ key_words[i - 1]
        elif (i - (i // 8) * 8) == 4:
            key_words[i] = BitVector(size=0)
            for j in range(4):
                key_words[i] += BitVector(intVal=
                                          byte_sub_table[key_words[i - 1][8 * j:8 * j + 8].intValue()], size=8)
            key_words[i] ^= key_words[i - 8]
        elif ((i - (i // 8) * 8) > 4) and ((i - (i // 8) * 8) < 8):
            key_words[i] = key_words[i - 8] ^ key_words[i - 1]
        else:
            sys.exit("error in key scheduling algo for i = %d" % i)
    return key_words


def gen_subbytes_table():
    subBytesTable = []
    c = BitVector(bitstring='01100011')
    for i in range(0, 256):
        a = BitVector(intVal=i, size=8).gf_MI(AES_modulus, 8) if i != 0 else BitVector(intVal=0)
        a1, a2, a3, a4 = [a.deep_copy() for x in range(4)]
        a ^= (a1 >> 4) ^ (a2 >> 5) ^ (a3 >> 6) ^ (a4 >> 7) ^ c
        subBytesTable.append(int(a))
    return subBytesTable


def gee(keyword, round_constant, byte_sub_table):
    rotated_word = keyword.deep_copy()
    rotated_word << 8
    newword = BitVector(size=0)
    for i in range(4):
        newword += BitVector(intVal=byte_sub_table[rotated_word[8 * i:8 * i + 8].intValue()], size=8)
    newword[:8] ^= round_constant
    round_constant = round_constant.gf_multiply_modular(BitVector(intVal=0x02), AES_modulus, 8)
    return newword, round_constant


# This function was taken from my solution of HW4 (HW4_AES)
def get_current_key(key_s, i):
    current_key = key_s[(i * 4)] + key_s[(i * 4) + 1] + key_s[(i * 4) + 2] + key_s[(i * 4) + 3]
    return current_key


# This function was taken from my solution of HW4 (HW4_AES)
def get_current_key_inverse(key_s, i):
    current_key = key_s[56 - (i * 4)] + key_s[56 - (i * 4) + 1] + key_s[56 - (i * 4) + 2] + key_s[56 - (i * 4) + 3]
    return current_key


# This function was taken from my solution of HW4 (HW4_AES)
def get_state_array(input_bv):
    state_array = [[0 for x in range(4)] for x in range(4)]
    for i in range(4):
        for j in range(4):
            a = j * 32 + i * 8
            b = a + 8
            state_array[i][j] = input_bv[a: b]
    return state_array


# This function was taken from my solution of HW4 (HW4_AES)
def subsitute(state_array):
    for i in range(4):
        for j in range(4):
            state_array[i][j] = BitVector(intVal=subBytesTable[int(state_array[i][j])], size=8)
    return state_array


# This function was taken from my solution of HW4 (HW4_AES)
def shift_row(state_array):
    temp = state_array[1][0]
    state_array[1][0] = state_array[1][1]
    state_array[1][1] = state_array[1][2]
    state_array[1][2] = state_array[1][3]
    state_array[1][3] = temp

    temp = state_array[2][0]
    state_array[2][0] = state_array[2][2]
    state_array[2][2] = temp
    temp = state_array[2][1]
    state_array[2][1] = state_array[2][3]
    state_array[2][3] = temp

    temp = state_array[3][0]
    state_array[3][0] = state_array[3][3]
    state_array[3][3] = state_array[3][2]
    state_array[3][2] = state_array[3][1]
    state_array[3][1] = temp
    return state_array


# This function was taken from my solution of HW4 (HW4_AES)
def mix_column_helper(state_array, mix_helper, a, b, c, d):
    two = BitVector(hexstring='02')
    three = BitVector(hexstring='03')
    for j in range(4):
        bv1 = mix_helper[a][j].gf_multiply_modular(two, AES_modulus, 8)
        bv2 = mix_helper[b][j].gf_multiply_modular(three, AES_modulus, 8)
        new_saj = bv1 ^ bv2 ^ mix_helper[c][j] ^ mix_helper[d][j]
        state_array[a][j] = new_saj

    return state_array, mix_helper


# This function was taken from my solution of HW4 (HW4_AES)
def mix_column(state_array):
    mix_helper = test_state_array_copy(state_array)

    # saj = (0x02 x saj) ^ (0x03 x sbj) ^ scj ^ sdj
    state_array, mix_helper = mix_column_helper(state_array, mix_helper, 0, 1, 2, 3)
    state_array, mix_helper = mix_column_helper(state_array, mix_helper, 1, 2, 0, 3)
    state_array, mix_helper = mix_column_helper(state_array, mix_helper, 2, 3, 0, 1)
    state_array, mix_helper = mix_column_helper(state_array, mix_helper, 3, 0, 1, 2)
    return state_array


# This function was taken from my solution of HW4 (HW4_AES)
def test_state_array_step(state_array):
    # initialize empty BitVector and fill it
    output_bv = BitVector(size=0)
    for i in range(4):
        for j in range(4):
            bv_to_out = state_array[j][i]
            output_bv += bv_to_out
    return output_bv


# This function was taken from my solution of HW4 (HW4_AES)
def test_state_array_copy(state_array):
    copy_array = [[0 for x in range(4)] for y in range(4)]
    for i in range(4):
        for j in range(4):
            copy_array[i][j] = state_array[i][j]
    return copy_array


# This function was taken from my solution of HW4 (HW4_AES)
# I made small modifications to take iv instead of direct plaintext
def encrypt(input_bv, key_schedule):
    if input_bv.length() < 128:
        input_bv.pad_from_right(128 - input_bv.length())

    # add first key
    current_key = get_current_key(key_schedule, 0)
    output_bv = input_bv ^ current_key
    # before = output_bv.get_bitvector_in_hex() # uncomment to see hex after first key added

    # implement the 14 rounds of encryption
    for i in range(0, 14):

        # just check what the starting state_array is
        state_array = get_state_array(output_bv)

        # substitution step
        state_array = subsitute(state_array)

        # shift row step
        state_array = shift_row(state_array)

        # mix column step
        if i != 13:
            state_array = mix_column(state_array)

        # add round key step
        output_bv = test_state_array_step(state_array)
        current_key = get_current_key(key_schedule, i + 1)
        output_bv = output_bv ^ current_key
        output_to_file = output_bv.get_bitvector_in_hex()

    return output_bv


# This function was created for the purpose of this homework
def ctr_aes_image(iv, image_file='image.ppm', out_file='enc_image.ppm', key_file='key.txt'):
    # gen sub tables
    genTables()

    # similar to DES_image homework
    FILEOUT = open(out_file, 'wb')

    # get header from original file
    bv = BitVector(filename=image_file)
    num_new_line = 0
    while bv.more_to_read:
        bitvec = bv.read_bits_from_file(8)
        bitvec.write_to_file(FILEOUT)
        bit = bitvec.get_bitvector_in_ascii()
        if bit == '\n':
            num_new_line += 1
        if num_new_line == 3:
            break

    # get key schedule, same as HW4
    key_bv = BitVector(filename=key_file)
    key_bv = key_bv.read_bits_from_file(256)
    key_schedule = gen_key_schedule_256(key_bv)
    counter = 0
    # i = -128
    while bv.more_to_read:
        bitvec = bv.read_bits_from_file(128)
        length = bitvec.length()
        if length > 0:
            if length < 128:
                bitvec.pad_from_right(128 - length)
            # increment iv
            i_number = iv.int_val()
            i_number = i_number + counter
            i_number = BitVector(intVal=i_number)
            if i_number.length() < 128:
                i_number.pad_from_left(128 - i_number.length())

        # encrypt IV and XOR with block of image
        encrypted_iv = encrypt(i_number, key_schedule)
        encrypted_image = encrypted_iv ^ bitvec
        hex_hex = encrypted_image.get_hex_string_from_bitvector()

        # write to file and increment counter
        encrypted_image.write_to_file(FILEOUT)
        counter = counter + 1
    return


# Test case given to us
if __name__ == "__main__":
    iv = BitVector(textstring='computersecurity')  # iv will be 128 bits
    ctr_aes_image(iv, 'image.ppm', 'enc_image.ppm', 'keyCTR.txt')
