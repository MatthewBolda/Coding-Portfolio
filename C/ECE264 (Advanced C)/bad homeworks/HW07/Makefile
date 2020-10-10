FLAGS = -std=c99 -g -Wall -Wshadow --pedantic -Wvla -Werror
GCC = gcc $(CFLAGS)
SRCS = pe07.c main.c
OBJS = pe07.o main.o #$(SRCS:%.c=%.o)
VALGRIND = valgrind --tool=memcheck --leak-check=full

all: pe07 testcase

pe07:$(OBJS)
	$(GCC) $(OBJS) -c pe07

testcase: test1

.c.o:
	$(GCC) $(FLAGS) -c $*.c

test1:  pe07
	./pe07 testInt.txt -I 

clean:
	/bin/rm -f pe07.o pe07
	/bin/rm -f main.o
	/bin/rm -f input* output*




