
#include "find_match_in_file.h"
#include "manage_input.h"
#include "regular_expression.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool compare_regex_struct_to_line(char *pointer_to_line, Regex *head_of_linked_list, bool is_exclusive);

void convert_to_lower(char *str)
{
  int i;
  for (i = 0; str[i]; i++) {
    str[i] = tolower(str[i]);
  }
  return;
}

void convert_phrase_in_regex_struct_to_lower(Regex *phrase_struct_linked_list)
{
  Regex *regex_linked_list_ptr = phrase_struct_linked_list;
  while (regex_linked_list_ptr != NULL) {
    switch (regex_linked_list_ptr->type) {
      case REGULAR_CHAR:
        regex_linked_list_ptr->content.regular_char_content =
            tolower(regex_linked_list_ptr->content.regular_char_content);
        break;
      case SQUARE_BRACKETS:
        regex_linked_list_ptr->content.square_brackets_range_limits[0] =
            tolower(regex_linked_list_ptr->content.square_brackets_range_limits[0]);
        regex_linked_list_ptr->content.square_brackets_range_limits[1] =
            tolower(regex_linked_list_ptr->content.square_brackets_range_limits[1]);
        break;
      case PARENTHESES:
        convert_to_lower(regex_linked_list_ptr->content.parentheses_options[0]);
        convert_to_lower(regex_linked_list_ptr->content.parentheses_options[1]);
      default:
        break;
    }
    regex_linked_list_ptr = regex_linked_list_ptr->next;
  }
  return;
}

bool check_if_match_line(arguments_struct *my_grep_parameters, const char *line, Regex *phrase_struct_linked_list)
{
  bool match_in_file = false;
  // creating copy of the line to make changes accoring to flags, withouut
  // harming the line:
  char *copy_line = (char *)malloc((strlen(line) + 1) * sizeof(char));
  strcpy(copy_line, line);
  if (my_grep_parameters->ignore_capital_letters == true) {
    convert_to_lower(copy_line);
    convert_phrase_in_regex_struct_to_lower(phrase_struct_linked_list);
  }
  match_in_file =
      compare_regex_struct_to_line(copy_line, phrase_struct_linked_list, my_grep_parameters->print_exclusive);
  free(copy_line);
  if (my_grep_parameters->print_if_not_in_line) {
    match_in_file = !match_in_file;
  }
  return match_in_file;
}

bool compare_regex_struct_to_line(char *pointer_to_line, Regex *head_of_linked_list, bool is_exclusive)
{
  char *iteration_pointer = pointer_to_line;
  bool flag_is_match = false;

  // for every char in the line string we want to compare to the linked list of
  // regex structs if we got to the end of the phrase, it means that all
  // matched, then break the traversal of the line for the case of exclusive
  // matching we want to compare only once, so we use do-while
  do {
    if (compare_regex_recursive(iteration_pointer, head_of_linked_list, is_exclusive)) {
      flag_is_match = true;
      break;
    }

    iteration_pointer++;
  } while (*iteration_pointer != '\0' && !is_exclusive);

  return flag_is_match;
}
