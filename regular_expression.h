#ifndef REGULAR_EXPRESSION
#define REGULAR_EXPRESSION
#include <stdbool.h>
#define NUM_OF_OPTIONS 2

typedef enum _regex_type { REGULAR_CHAR, DOT, PARENTHESES, SQUARE_BRACKETS, END } regex_type;

typedef union _content_of_regex {
  char regular_char_content;
  char *parentheses_options[NUM_OF_OPTIONS];
  int square_brackets_range_limits[NUM_OF_OPTIONS];
} RegexContent;

typedef struct _regex {
  regex_type type;
  RegexContent content;
  int len;
  struct _regex *next;
} Regex;

Regex *get_regex_linked_list_from_phrase(char *phrase_to_match);
void free_regex_linked_list(Regex *head);

#endif  // !REGULAR_EXPRESSION
