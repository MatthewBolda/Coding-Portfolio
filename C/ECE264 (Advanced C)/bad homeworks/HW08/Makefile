CFLAGS = -std=c99 -g -Wall -Wshadow --pedantic -Wvla -Werror
GCC = gcc $(CFLAGS) 

SRCS = pa08.c main.c
OBJS = $(SRCS:%.c=%.o)
VALGRIND = valgrind --tool=memcheck --leak-check=full

all: pa08 testcase

pa08:$(OBJS)
	$(GCC) $(OBJS) $(KMEANOBJFILE) -o pa08

.c.o:
	$(GCC) $(FLAGS) -c $*.c

testcase: test1 test2 test3 test4 test5 test6 test7 test8 test9 test10


test1: pa08
	./pa08 -D 0 2 3 > outDouble.txt


test2: pa08
	./pa08 -D 0 2 3 | sort > outSortedDouble.txt

test3: pa08
	./pa08 -D 4 9 5 > outDouble2.txt

test4: pa08
	./pa08 -D 4 9 5 | sort > outSortedDouble2.txt

test5: pa08
	./pa08 -D 3 6 4 > outDouble3.txt

test6: pa08
	./pa08 -D 3 6 4 | sort > outSortedDouble3.txt

test7: pa08
	./pa08 -VID a b C D 1 3 2 > outVID.txt
	
test8: pa08
	./pa08 -VID a b C D 1 3 2 | sort > outSortedVID.txt
clean:
	/bin/rm -f input*
	/bin/rm -f pa08.o
	/bin/rm -f pa08
	/bin/rm -f output*
