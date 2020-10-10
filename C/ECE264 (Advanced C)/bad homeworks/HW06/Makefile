CFLAGS = -std=c99 -g -Wall -Wshadow -Wvla -Werror
GCC = gcc $(CFLAGS)
SRCS = pa06.c main.c
OBJS = $(SRCS:%.c=%.o)
VALGRIND = valgrind --tool=memcheck --leak-check=full -v --track-origins=yes

all = pa06 testcase

pa06:$(OBJS)
	$(GCC) $(OBJS) -o pa06

testcase: test1 test2 test3 test4 test5

.c.o:
	$(GCC) $(FLAGS) -c $*.c

test1:
	$(VALGRIND) ./pa06 SELECT name age WHERE age ">" 10 AND id = 1

test2:
	$(VALGRIND) ./pa06 SELECT age name WHERE age ">" 10 AND id = 1

test3:
	 ./pa06 SELECT enrollment age major WHERE age "<" 22 AND id = 100

test4:
	./pa06 SELECT id WHERE enrollment = Yes

test5:
	./pa06 SELECT id WHERE enrollment = Yes AND name ==aa

clean:
	/bin/rm -f pa06.o pa06
	/bin/rm -f input* output*
