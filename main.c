#include "activate_grep_structure.h"
#include "manage_input.h"
#include "regular_expression.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
  check_num_of_arguments(argc);
  arguments_struct *new_grep = initial_grep_parameters();
  new_grep = identify_grep_options(new_grep, argc, argv);
  Regex *phrase_struct_linked_list = get_regex_linked_list_from_phrase(new_grep->phrase);
  activate_grep(new_grep, phrase_struct_linked_list);
  free_regex_linked_list(phrase_struct_linked_list);
  free(new_grep);
  return 0;
}
