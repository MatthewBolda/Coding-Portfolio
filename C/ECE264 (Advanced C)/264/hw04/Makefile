#Variables
CC=gcc
CFLAGS=-g -std=c99 -Wall -Wshadow -Wvla -Werror -pedantic
SRC_C=mintf.c
SRC_H=mintf.h clog.h miniunit.h
TEST_C=test_mintf.c
TEST_EXPECTED=test_mintf.txt
TEST_ACTUAL=actual.txt
EXECUTABLE=test_mintf
ASG_NICKNAME=HW04

#Rules
$(EXECUTABLE): $(SRC_C) $(TEST_C) $(SRC_H)
	$(CC) -o test_mintf mintf.c test_mintf.c $(CFLAGS)
#test rule 2 here

submit: $(SRC_H) Makefile echo_color.c
	264submit HW05 miniunit.h clog.h Makefile echo_color.c

#not ready pretest:

