CFLAGS = -std=c99 -g -Wall -Wshadow --pedantic -Wvla -Werror
GCC = gcc $(CFLAGS)
SRCS = pe04.c main.c
OBJS = $(SRCS:%.c=%.o)
VALGRIND = valgrind --tool=memcheck --leak-check=full

all: pe04 testcase

pe04:$(OBJS)
	$(GCC) $(OBJS) -o pe04

testcase: test1

.c.o:
	$(GCC) $(FLAGS) -c $*.c

test1:	pe04
	./pe04 database.txt -a

clean:
	/bin/rm -f pe04.o pe04
	/bin/rm -f input* output*

