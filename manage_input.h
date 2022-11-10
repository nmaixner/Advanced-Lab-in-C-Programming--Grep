#ifndef MANAGE_INPUT_H
#define MANAGE_INPUT_H
#include <stdbool.h>

typedef struct arguments_struct {
  char *phrase;
  char *file_name;
  bool print_line_after;        // True if -A
  int num_of_line_to_print;     // num for -A
  bool num_of_bytes;            // True if -b
  bool print_num_of_lines;      // True if -c
  bool ignore_capital_letters;  // True if -i
  bool print_line_number;       // True if -n
  bool print_if_not_in_line;    // True if -v
  bool print_exclusive;         // True if -x
  bool special_signs;           // True if -E
} arguments_struct;

arguments_struct *identify_grep_options(arguments_struct *my_grep_parameters, int argc, char *argv[]);
arguments_struct *initial_grep_parameters();
void check_num_of_arguments(int argc);

#endif  // !MANAGE_INPUT_H
