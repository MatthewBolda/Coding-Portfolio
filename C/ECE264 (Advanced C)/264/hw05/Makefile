#Variables
CC=gcc
CFLAGS=-g -std=c99 -Wall -Wshadow -Wvla -Werror -pedantic
SRC_C=mintf.c
SRC_H=mintf.h clog.h miniunit.h
HEADERS_H=mintf.h clog.h
TEST_C=test_mintf.c
TEST_EXPECTED=test_mintf.txt
TEST_ACTUAL=actual.txt
EXECUTABLE=test_mintf
ASG_NICKNAME=HW04
COVERAGE=-ftest-coverage -fprofile-arcs -DNDEBUG
ECHO_C=echo_color.c
DEBUG=-DDEBUG
CFLAGS_GCOV=$(CFLAGS) -fprofile-arcs -ftest-coverage

#Rules
$(EXECUTABLE): $(SRC_C) $(TEST_C) $(HEADERS_H)
	$(CC) -o test_mintf mintf.c test_mintf.c $(CFLAGS)

test: $(EXECUTABLE) echo_color $(TEST_EXPECTED) $(TEST_ACTUAL)
	./test_mintf > actual.txt
	diff -b actual.txt test_mintf.txt &> /dev/null
	if [ $$? -eq 0]; then\
			./echo_color green "Test passed: $(TEST_ACTUAL) matches $(TEST_EXPECTED)";\
		else\
			./echo_color red "Test failed: $(TEST_ACTUAL) does NOT match $(TEST_EXPECTED)";\
		fi


submit: $(SRC_H) $(SRC_C) $(TEST_C) $(TEST_EXPECTED)
	264submit HW04 mintf.c test_mintf.c test_mintf.txt miniunit.h clog.h

pretest: $(SRC_H) $(SRC_C) $(TEST_C) $(TEST_EXPECTED)
	264test hw04

coverage: $(SRC_C) $(TEST_C)
	$(CC) -o test_mintf mintf.c test_mintf.c $(CFLAGS_GCOV) -DNDEBUG
	./test_mintf
	gcov -f mintf.c

debug: $(TEST_C) $(SRC_H)
	$(CC) -o test_mintf mintf.c $(TEST_C) $(CFLAGS) $(DEBUG)


echo_color: $(ECHO_C)
	$(CC) -o echo_color echo_color.c $(CFLAGS)


clean:
	rm -f *.o actual.txt

#PHONY
.PHONY: actual.txt


