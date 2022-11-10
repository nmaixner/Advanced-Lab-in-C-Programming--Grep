#include "activate_grep_structure.h"
#include "find_match_in_file.h"
#include "handle_prints.h"
#include "manage_input.h"
#include "regular_expression.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE *get_ptr(arguments_struct *my_grep)
{
  FILE *file_ptr = NULL;
  if (NULL == my_grep->file_name) {
    file_ptr = stdin;
  } else {
    file_ptr = fopen(my_grep->file_name, "r");
  }
  return file_ptr;
}

void activate_grep(arguments_struct *arguments_struct, Regex *phrase_struct_linked_list)
{
  FILE *file_ptr = get_ptr(arguments_struct);
  char *line_in_file = NULL;
  size_t line_size = 0;
  bool is_line_for_printing, check_if_block_over = false;
  int num_line = 1, num_of_lines = 0, num_bytes = 0, counter_num_of_line_to_print = 0;

  while (getline(&line_in_file, &line_size, file_ptr) != EOF) {
    is_line_for_printing = check_if_match_line(arguments_struct, line_in_file, phrase_struct_linked_list);
    if (is_line_for_printing) {
      if (true == check_if_block_over) {
        printf("--\n");
        check_if_block_over = false;
      }
      counter_num_of_line_to_print = 1;
      num_of_lines++;
      handle_lines_prints(line_in_file, arguments_struct, num_line, num_bytes, ":");
    } else if (counter_num_of_line_to_print == (arguments_struct->num_of_line_to_print + 1)) {
      counter_num_of_line_to_print = 0;
      check_if_block_over = true;
    } else if (arguments_struct->print_line_after && counter_num_of_line_to_print > 0) {
      handle_lines_prints(line_in_file, arguments_struct, num_line, num_bytes, "-");
      counter_num_of_line_to_print++;
    }
    num_bytes += strlen(line_in_file);
    num_line++;
  }
  if (NULL != line_in_file) {
    free(line_in_file);
  }
  if (arguments_struct->print_num_of_lines == true) {
    printf("%d\n", num_of_lines);
  }
  fclose(file_ptr);
  return;
}
