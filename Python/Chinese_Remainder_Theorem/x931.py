# !/usr/bin/env python3
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


# This function is from my AES.py from last homework (HW4_AES)
def get_state_array(input_bv):
    state_array = [[0 for x in range(4)] for x in range(4)]
    for i in range(4):
        for j in range(4):
            a = j * 32 + i * 8
            b = a + 8
            state_array[i][j] = input_bv[a: b]
    return state_array


# This function is from my AES.py from last homework (HW4_AES)
def subsitute(state_array):
    for i in range(4):
        for j in range(4):
            state_array[i][j] = BitVector(intVal=subBytesTable[int(state_array[i][j])], size=8)
    return state_array


# This function is from my AES.py from last homework (HW4_AES)
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


# This function is from my AES.py from last homework (HW4_AES)
def mix_column_helper(state_array, mix_helper, a, b, c, d):
    two = BitVector(hexstring='02')
    three = BitVector(hexstring='03')
    for j in range(4):
        bv1 = mix_helper[a][j].gf_multiply_modular(two, AES_modulus, 8)
        bv2 = mix_helper[b][j].gf_multiply_modular(three, AES_modulus, 8)
        new_saj = bv1 ^ bv2 ^ mix_helper[c][j] ^ mix_helper[d][j]
        state_array[a][j] = new_saj

    return state_array, mix_helper


# This function is from my AES.py from last homework (HW4_AES)
def mix_column(state_array):
    mix_helper = test_state_array_copy(state_array)

    # saj = (0x02 x saj) ^ (0x03 x sbj) ^ scj ^ sdj
    state_array, mix_helper = mix_column_helper(state_array, mix_helper, 0, 1, 2, 3)
    state_array, mix_helper = mix_column_helper(state_array, mix_helper, 1, 2, 0, 3)
    state_array, mix_helper = mix_column_helper(state_array, mix_helper, 2, 3, 0, 1)
    state_array, mix_helper = mix_column_helper(state_array, mix_helper, 3, 0, 1, 2)
    return state_array


# This function is from my AES.py from last homework (HW4_AES)
def test_state_array_step(state_array):
    # initialize empty BitVector and fill it
    output_bv = BitVector(size=0)
    for i in range(4):
        for j in range(4):
            bv_to_out = state_array[j][i]
            output_bv += bv_to_out
    return output_bv


# This function is from my AES.py from last homework (HW4_AES)
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


def x931(v0, dt, totalNum, key_file):
    # initialize helper items
    genTables()
    return_list = []

    # get key_schedule
    key_bv = BitVector(filename=key_file)
    key_bv = key_bv.read_bits_from_file(256)
    key_schedule = gen_key_schedule_256(key_bv)

    for i in range(totalNum):
        # get random number
        EDE_dt = encrypt(dt, key_schedule)
        XOR_Vj = v0 ^ EDE_dt
        random_number_Rj = encrypt(XOR_Vj, key_schedule)

        # get next v0
        new_vj = random_number_Rj ^ EDE_dt
        next_v0 = encrypt(new_vj, key_schedule)
        v0 = next_v0

        # add random number to list
        return_list.append(int(random_number_Rj.get_bitvector_in_hex(), 16))

    return return_list


# Test case given to us
if __name__ == "__main__":
    v0 = BitVector(textstring='computersecurity')  # v0 will be 128 bits
    # As mentioned before, for testing purposes dt is set to a predetermined value
    dt = BitVector(intVal=99, size=128)
    listX931 = x931(v0, dt, 3, 'keyX931.txt')
    # Check if list is correct
    print('{}\n{}\n{}'.format(int(listX931[0]), int(listX931[1]), int(listX931[2])))
