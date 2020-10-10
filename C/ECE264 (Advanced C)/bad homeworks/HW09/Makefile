FLAGS = -std=c99 -g -Wall -Wshadow --pedantic -Wvla -Werror
GCC = gcc $(CFLAGS)

SRCS = pa09.c main.c read_write.c
OBJS = $(SRCS:%.c=%.o)
VALGRIND = valgrind --tool=memcheck --leak-check=full

all: pa09 testcase

pa09:$(OBJS)
	$(GCC) $(OBJS) $(KMEANOBJFILE) -o pa09

.c.o:
	$(GCC) $(FLAGS) -c $*.c

testcase: test1 test2 test3 test4 test5

test1: pa09
	./pa09 databaseBinary.bin -a outputSortAge.bin > outputPivotsAge.txt

test2: pa09
	./pa09 databaseBinary.bin -n outputSortName.bin > outputPivotsName.txt

test3: pa09
	valgrind --tool=memcheck --log-file=memcheckage.txt --leak-check=full --verbose ./pa09 databaseBinary.bin -a filename

test4: pa09
	valgrind --tool=memcheck --log-file=memcheckage.txt --leak-check=full --verbose ./pa09 databaseBinary.bin -n filename

test5: pa09
	memcheckage.txt - valgrind --tool=memcheck --log-file=memcheckage.txt --leak-check=full --verbose ./pa09 databaseBinary.bin -n sortbyname.bin > expectedPivotAge.txt

#test5: pa09
#	./pa09 smallDbBinary.bin -n sortSmallbyname.bin > expectedSmallPivotName.txt

test6: pa09
	./pa09 smallDbBinary.bin -a sortSmallbyage.bin
	diff -i -w outputSortAge.bin sortbyage.bin
clean:
	/bin/rm -f input*
	/bin/rm -f pa09.o
	/bin/rm -f pa09
	/bin/rm -f output*

