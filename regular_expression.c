#include "regular_expression.h"
#include "find_match_in_file.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SIZE_OF_SQUARE_BRACKETS 5
#define SIZE_OF_DOT 1
#define SIZE_OF_CHAR 1
#define SIZE_OF_PARENTHESES 3

Regex *new_regex_item(regex_type type)
{
  Regex *new_item = (Regex *)malloc(sizeof(Regex));
  new_item->type = type;
  new_item->next = NULL;
  return new_item;
}

regex_type identify_regex_type(char regex_first_char)
{
  regex_type identified_type;
  switch (regex_first_char) {
    case '.':
      identified_type = DOT;
      break;

    case '(':
      identified_type = PARENTHESES;
      break;

    case '[':
      identified_type = SQUARE_BRACKETS;
      break;
    case '\0':
      identified_type = END;
      break;
    default:
      identified_type = REGULAR_CHAR;
      break;
  }
  return identified_type;
}

char *get_sliced_string_array(char *pointer_to_expression, char delimiter)
{
  char *sliced_string = (char *)malloc(strlen(pointer_to_expression) + 1);
  int i = 0;
  while (*pointer_to_expression != delimiter) {
    *(sliced_string + i) = *pointer_to_expression;
    pointer_to_expression++;
    i++;
  }
  *(sliced_string + i) = '\0';
  return sliced_string;
}

Regex *get_regex_content_and_len(Regex *new_parsed_part, char *pointer_to_expression)
{
  char *copy_pointer_to_expression = pointer_to_expression;
  int len_sliced_str_option1 = 0, len_sliced_str_option2 = 0;
  switch (new_parsed_part->type) {
    case REGULAR_CHAR:
      new_parsed_part->content.regular_char_content = *copy_pointer_to_expression;
      new_parsed_part->len = SIZE_OF_CHAR;
      break;
    case SQUARE_BRACKETS:
      copy_pointer_to_expression++;  // the first char is '[' , need to increment
                                     // pointer one position
      new_parsed_part->content.square_brackets_range_limits[0] = *copy_pointer_to_expression;
      copy_pointer_to_expression += 2;  // the next char is '-' , need to increment pointer two position
      new_parsed_part->content.square_brackets_range_limits[1] = *copy_pointer_to_expression;
      new_parsed_part->len = SIZE_OF_SQUARE_BRACKETS;
      break;
    case PARENTHESES:
      copy_pointer_to_expression++;  // the current char is '(' , need to increment
                                     // pointer one position
      new_parsed_part->content.parentheses_options[0] = get_sliced_string_array(copy_pointer_to_expression, '|');
      // the first sliced string delimited by '|'
      len_sliced_str_option1 = strlen(new_parsed_part->content.parentheses_options[0]);
      copy_pointer_to_expression += len_sliced_str_option1 + 1;
      // the current char is '|' , need to increment pointer by another one
      // position
      new_parsed_part->content.parentheses_options[1] = get_sliced_string_array(copy_pointer_to_expression, ')');
      // the second sliced string end in char ')'
      len_sliced_str_option2 = strlen(new_parsed_part->content.parentheses_options[1]);
      new_parsed_part->len = len_sliced_str_option1 + len_sliced_str_option2 + SIZE_OF_PARENTHESES;
      break;
    case DOT:
      new_parsed_part->len = SIZE_OF_DOT;
      break;
    default:
      break;
  }
  return new_parsed_part;
}

Regex *get_regex_linked_list_from_phrase(char *phrase_to_match)
{
  char *pointer_to_phrase = phrase_to_match;
  regex_type type;
  Regex *new_parsed_part = NULL, *regex_head = NULL, *previous_regex = NULL;
  ;
  bool is_escaped = false;

  while (*pointer_to_phrase != '\0') {
    if (*pointer_to_phrase == '\\' && is_escaped == false) {
      is_escaped = true;
      pointer_to_phrase++;
      continue;
    }
    type = identify_regex_type(*pointer_to_phrase);
    if (is_escaped == true) {
      type = REGULAR_CHAR;
      is_escaped = false;
    }
    if (new_parsed_part != NULL) {
      previous_regex = new_parsed_part;
    }
    new_parsed_part = new_regex_item(type);
    new_parsed_part = get_regex_content_and_len(new_parsed_part, pointer_to_phrase);
    if (regex_head == NULL) {
      regex_head = new_parsed_part;
    }
    if (previous_regex != NULL) {
      previous_regex->next = new_parsed_part;
    }
    pointer_to_phrase += new_parsed_part->len;
  }
  previous_regex = new_parsed_part;
  type = END;
  Regex *tail_parsed_part = new_regex_item(type);
  previous_regex->next = tail_parsed_part;
  return regex_head;
}

bool compare_regex_recursive(char *pointer_to_line_segment, Regex *head_of_linked_list, bool is_exclusive)
{
  char *iteration_pointer = pointer_to_line_segment, lower_limit, upper_limit, *expression0, *expression1, c;
  Regex *regex_to_check = head_of_linked_list;
  bool is_matching = false, is_char_equal, match1 = false, match2 = false;
  if ((*iteration_pointer == '\0' || *iteration_pointer == '\n') && (regex_to_check->type != END)) {
    return false;
  }
  switch (regex_to_check->type) {
    case REGULAR_CHAR:
      is_char_equal = (regex_to_check->content.regular_char_content == *iteration_pointer);
      is_matching = is_char_equal && compare_regex_recursive(iteration_pointer + 1, regex_to_check->next, is_exclusive);
      break;
    case DOT:
      is_matching = compare_regex_recursive(iteration_pointer + 1, regex_to_check->next, is_exclusive);
      break;
    case PARENTHESES:
      expression0 = regex_to_check->content.parentheses_options[0],
      expression1 = regex_to_check->content.parentheses_options[1];
      int len_expression0 = strlen(expression0), len_expression1 = strlen(expression1);
      if (strncmp(expression0, iteration_pointer, len_expression0) == 0) {
        match1 = compare_regex_recursive(iteration_pointer + len_expression0, regex_to_check->next, is_exclusive);
      }
      if (strncmp(expression1, iteration_pointer, len_expression1) == 0) {
        match2 = compare_regex_recursive(iteration_pointer + len_expression1, regex_to_check->next, is_exclusive);
      }
      is_matching = match1 || match2;
      break;
    case SQUARE_BRACKETS:
      lower_limit = regex_to_check->content.square_brackets_range_limits[0],
      upper_limit = regex_to_check->content.square_brackets_range_limits[1];
      for (c = lower_limit; c < (upper_limit + 1); c++) {
        is_char_equal = (c == *iteration_pointer);
        is_matching =
            is_char_equal && compare_regex_recursive(iteration_pointer + 1, regex_to_check->next, is_exclusive);
        if (is_matching) {
          break;
        }
      }
      break;
    case END:
      if (!is_exclusive || (is_exclusive && *iteration_pointer == '\0') ||
          (is_exclusive && *iteration_pointer == '\n')) {
        is_matching = true;
      }
  }
  return is_matching;
}

void free_regex_linked_list(Regex *head)
{
  Regex *next_regex_to_free;
  Regex *ptr_to_free = head;
  int i;
  while (ptr_to_free != NULL) {
    next_regex_to_free = ptr_to_free->next;
    if (ptr_to_free->type == PARENTHESES) {
      for (i = 0; i < NUM_OF_OPTIONS; i++) {
        free(ptr_to_free->content.parentheses_options[i]);
      }
    }
    free(ptr_to_free);
    ptr_to_free = next_regex_to_free;
  }
}
