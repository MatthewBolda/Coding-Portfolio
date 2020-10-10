CFLAGS = -std=c99 -g -Wall -Wshadow --pedantic -Wvla -Werror
GCC = gcc $(CFLAGS)
SRCS = pe05.c main.c
OBJFILES = obj/pe04.o
OBJS = $(SRCS:%.c=%.o)
VALGRIND = valgrind --tool=memcheck --leak-check=full -v --track-origins=yes

all: pe05 testcase

pe05:$(OBJS)
	$(GCC) $(OBJS) $(OBJFILES) -o pe05

.c.o:
	$(GCC) $(FLOGS) -c $*.c

testcase: test1 test2

test1: pe05
	./pe05 database.txt -a sortbyage.txt
	diff output1.txt sortbyage.txt
test2: pe05
	./pe05 database.txt -n sortbyname.txt

clean:
	/bin/rm -f pe05.o pe05
	/bin/rm -f input* output* 
