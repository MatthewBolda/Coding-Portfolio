#!/usr/bin/env python
# Matthew Bolda

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


def get_current_key(key_s, i):
    current_key = key_s[(i * 4)] + key_s[(i * 4) + 1] + key_s[(i * 4) + 2] + key_s[(i * 4) + 3]
    return current_key


def get_current_key_inverse(key_s, i):
    current_key = key_s[56 - (i * 4)] + key_s[56 - (i * 4) + 1] + key_s[56 - (i * 4) + 2] + key_s[56 - (i * 4) + 3]
    return current_key


def get_state_array(input_bv):
    state_array = [[0 for x in range(4)] for x in range(4)]
    for i in range(4):
        for j in range(4):
            a = j * 32 + i * 8
            b = a + 8
            state_array[i][j] = input_bv[a: b]
    return state_array


def subsitute(state_array):
    for i in range(4):
        for j in range(4):
            state_array[i][j] = BitVector(intVal=subBytesTable[int(state_array[i][j])], size=8)
    return state_array


def subsitute_inverse(state_array):
    for i in range(4):
        for j in range(4):
            state_array[i][j] = BitVector(intVal=invSubBytesTable[int(state_array[i][j])], size=8)
    return state_array


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


def shift_row_inverse(state_array):
    temp = state_array[1][0]
    state_array[1][0] = state_array[1][3]
    state_array[1][3] = state_array[1][2]
    state_array[1][2] = state_array[1][1]
    state_array[1][1] = temp

    temp = state_array[2][0]
    state_array[2][0] = state_array[2][2]
    state_array[2][2] = temp
    temp = state_array[2][1]
    state_array[2][1] = state_array[2][3]
    state_array[2][3] = temp

    temp = state_array[3][0]
    state_array[3][0] = state_array[3][1]
    state_array[3][1] = state_array[3][2]
    state_array[3][2] = state_array[3][3]
    state_array[3][3] = temp
    return state_array


def mix_column_helper(state_array, mix_helper, a, b, c, d):
    two = BitVector(hexstring='02')
    three = BitVector(hexstring='03')
    for j in range(4):
        bv1 = mix_helper[a][j].gf_multiply_modular(two, AES_modulus, 8)
        bv2 = mix_helper[b][j].gf_multiply_modular(three, AES_modulus, 8)
        new_saj = bv1 ^ bv2 ^ mix_helper[c][j] ^ mix_helper[d][j]
        state_array[a][j] = new_saj

    return state_array, mix_helper


def mix_column(state_array):
    mix_helper = test_state_array_copy(state_array)

    # saj = (0x02 x saj) ^ (0x03 x sbj) ^ scj ^ sdj
    state_array, mix_helper = mix_column_helper(state_array, mix_helper, 0, 1, 2, 3)
    state_array, mix_helper = mix_column_helper(state_array, mix_helper, 1, 2, 0, 3)
    state_array, mix_helper = mix_column_helper(state_array, mix_helper, 2, 3, 0, 1)
    state_array, mix_helper = mix_column_helper(state_array, mix_helper, 3, 0, 1, 2)
    return state_array


def mix_column_helper_inverse(state_array, mix_helper, a, b, c, d):
    Oe = BitVector(hexstring='0e')
    Ob = BitVector(hexstring='0b')
    Od = BitVector(hexstring='0d')
    O9 = BitVector(hexstring='09')
    for j in range(4):
        bv1 = mix_helper[a][j].gf_multiply_modular(Oe, AES_modulus, 8)
        bv2 = mix_helper[b][j].gf_multiply_modular(Ob, AES_modulus, 8)
        bv3 = mix_helper[c][j].gf_multiply_modular(Od, AES_modulus, 8)
        bv4 = mix_helper[d][j].gf_multiply_modular(O9, AES_modulus, 8)
        new_saj = bv1 ^ bv2 ^ bv3 ^ bv4
        state_array[a][j] = new_saj

    return state_array, mix_helper


def mix_column_inverse(state_array):
    mix_helper = test_state_array_copy(state_array)

    # saj = (0x02 x saj) ^ (0x03 x sbj) ^ scj ^ sdj
    state_array, mix_helper = mix_column_helper_inverse(state_array, mix_helper, 0, 1, 2, 3)
    state_array, mix_helper = mix_column_helper_inverse(state_array, mix_helper, 1, 2, 3, 0)
    state_array, mix_helper = mix_column_helper_inverse(state_array, mix_helper, 2, 3, 0, 1)
    state_array, mix_helper = mix_column_helper_inverse(state_array, mix_helper, 3, 0, 1, 2)
    return state_array


# used for debugging
def test_state_array_step(state_array):
    # initialize empty BitVector and fill it
    output_bv = BitVector(size=0)
    for i in range(4):
        for j in range(4):
            bv_to_out = state_array[j][i]
            output_bv += bv_to_out
    return output_bv


# used to create a copy of a state array before I remembered deepcopy works
def test_state_array_copy(state_array):
    copy_array = [[0 for x in range(4)] for y in range(4)]
    for i in range(4):
        for j in range(4):
            copy_array[i][j] = state_array[i][j]
    return copy_array


def encrypt(inputfile, keyfile, outputfile):
    # get the key schedule
    key_bv = BitVector(filename=keyfile)
    key_bv = key_bv.read_bits_from_file(256)
    key_schedule = gen_key_schedule_256(key_bv)

    # open the output file for writing
    output_file = open(outputfile, 'w')

    # chop up the file into blocks of 128 bits
    input = BitVector(filename=inputfile)
    while input.more_to_read:
        input_bv = input.read_bits_from_file(128)
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
            ''' # test to see get_state_array is working properly
            starting_state_array = test_state_array_copy(state_array)
            starting_test = test_state_array_step(starting_state_array)
            starting_hex = starting_test.get_bitvector_in_hex()
            '''

            # substitution step
            state_array = subsitute(state_array)
            ''' # test to see subsitute is working properly
            substitute_state_array = test_state_array_copy(state_array)
            substitute_test = test_state_array_step(substitute_state_array)
            substitute_hex = substitute_test.get_bitvector_in_hex()
            '''

            # shift row step
            state_array = shift_row(state_array)
            ''' # test to see shift_row is working properly
            shifted_state_array = test_state_array_copy(state_array)
            shifted_test = test_state_array_step(shifted_state_array)
            shifted_hex = shifted_test.get_bitvector_in_hex()
            '''
            # mix column step
            if i != 13:
                state_array = mix_column(state_array)
                ''' # test to see mix_column is working properly
                mixed_state_array = test_state_array_copy(state_array)
                mixed_test = test_state_array_step(mixed_state_array)
                mixed_hex = mixed_test.get_bitvector_in_hex()
                '''

            # add round key step
            output_bv = test_state_array_step(state_array)
            current_key = get_current_key(key_schedule, i + 1)
            output_bv = output_bv ^ current_key
            output_to_file = output_bv.get_bitvector_in_hex()

        # write to file
        output_file.write(output_to_file)
    output_file.close()

    return


def decrypt(inputfile, keyfile, outputfile):
    # create key schedule
    key_bv = BitVector(filename=keyfile)
    key_bv = key_bv.read_bits_from_file(256)
    key_schedule = gen_key_schedule_256(key_bv)

    # open both files for reading and writing
    output_file = open(outputfile, 'w')
    input_file = open(inputfile, 'r')
    encrypted = input_file.read()
    encrypted = BitVector(
        hexstring=encrypted)  # needs to be hexstring because that was the required output in encrpytion
    input_file.close()

    # create ending conditions
    location = 0
    block_length = 128
    ender_condition = encrypted.length() / block_length

    # continuing until condition meet
    while location != ender_condition:
        if location == ender_condition:
            break

        # get next 128 bits for decrypting
        input_bv = encrypted[location * block_length:min((location + 1) * block_length, encrypted.length())]
        if input_bv.length() < 128:
            input_bv.pad_from_right(128 - input_bv.length())

        # add first key
        current_key = get_current_key_inverse(key_schedule, 0)
        output_bv = input_bv ^ current_key
        before = output_bv.get_bitvector_in_hex()

        # implement 14 round decrpytion
        for i in range(0, 14):
            # ''' sections are for testing step by step changes in hex codes.
            # convert output_bv into state_array
            state_array = get_state_array(output_bv)
            ''' # test if get_state_array is working properly
            starting_state_array = test_state_array_copy(state_array)
            starting_test = test_state_array_step(starting_state_array)
            starting_hex = starting_test.get_bitvector_in_hex()
            '''
            # shift row step
            state_array = shift_row_inverse(state_array)
            ''' # test if shift_row_inverse is working properly
            shifted_state_array = test_state_array_copy(state_array)
            shifted_test = test_state_array_step(shifted_state_array)
            shifted_hex = shifted_test.get_bitvector_in_hex()
            '''
            # substitution step
            state_array = subsitute_inverse(state_array)
            ''' # test if subsitute_inverse is working properly
            substitute_state_array = test_state_array_copy(state_array)
            substitute_test = test_state_array_step(substitute_state_array)
            substitute_hex = substitute_test.get_bitvector_in_hex()
            '''
            # add round key step
            output_bv = test_state_array_step(state_array)
            current_key = get_current_key_inverse(key_schedule, i + 1)
            output_bv = output_bv ^ current_key
            state_array = get_state_array(output_bv)

            # mix column step, do not mix on last round
            if i != 13:
                state_array = mix_column_inverse(state_array)
                ''' # test if mix_colum_inverse is working properly
                mixed_state_array = test_state_array_copy(state_array)
                mixed_test = test_state_array_step(mixed_state_array)
                mixed_hex = mixed_test.get_bitvector_in_hex()
                '''

            output_bv = test_state_array_step(state_array)
            # output_to_file = output_bv.get_bitvector_in_hex() # uncomment for hex output
        location = location + 1
        output_text = output_bv.get_text_from_bitvector()
        output_file.write(output_text)
    output_file.close()
    return

# argv[1] argv[2]       argv[3]   argv[4]
# -e      message.txt   key.txt   encrypted.txt
# -d      encrypted.txt key.txt   decrypted.txt
# flag    inputfile     keyfile   outputfile
flag = sys.argv[1]
inputfile = sys.argv[2]
keyfile = sys.argv[3]
outputfile = sys.argv[4]
if flag == '-e' or flag == '-d':
    genTables()
if flag == '-e':
    encrypt(inputfile, keyfile, outputfile)
elif flag == '-d':
    decrypt(inputfile, keyfile, outputfile)
