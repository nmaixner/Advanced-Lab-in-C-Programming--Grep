.PHONY: all clean test

clean:
	rm -f *.o my_grep

all:	my_grep


test: 	all
	/specific/a/home/cc/students/csguests/nimrodav/grep_tests/run_all.sh

CFLAGS=-Wall -Wextra -g -O1 -std=c99

my_grep: main.o activate_grep_structure.o find_match_in_file.o manage_input.o 
	gcc $(CFLAGS) main.o activate_grep_structure.o find_match_in_file.o manage_input.o my_grep.o

main.o: main.c manage_input.h activate_grep_structure.h find_match_in_file.h regular_expression.h
	gcc $(CFLAGS) -c main.c
	
find_match_in_file.o: find_match_in_file.c manage_input.h find_match_in_file.h regular_expression.h
	gcc $(CFLAGS) -c find_match_in_file.c


manage_input.o: manage_input.c manage_input.h
	gcc $(CFLAGS) -c manage_input.c
activate_grep_structure.o: activate_grep_structure.c activate_grep_structure.h manage_input.h find_match_in_file.h regular_expression.h
	gcc $(CFLAGS) -c activate_grep_structure.c

regular_expression.o: regular_expression.c regular_expression.h
	gcc $(CFLAGS) -c regular_expression.c
.PHONY: all clean test

