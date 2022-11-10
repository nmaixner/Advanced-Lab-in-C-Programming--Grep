#include "handle_prints.h"
#include "activate_grep_structure.h"
#include "regular_expression.h"
#include <stdio.h>
void handle_lines_prints(char const *line, arguments_struct *my_grep, int num_line, int num_bytes, char *sign)
{
  if (my_grep->print_num_of_lines == true) {
    return;
  } else {
    if (my_grep->num_of_bytes == true) {
      printf("%d%s", num_bytes, sign);
    } else if (my_grep->print_line_number == true) {
      printf("%d%s", num_line, sign);
    }
    printf("%s", line);
  }
  return;
}
