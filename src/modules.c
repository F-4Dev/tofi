#include "modules.h"
#include "history.h"
#include "string_vec.h"
#include "unicode.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void module_math_suggestion(const char *restrict query,
                            struct string_ref_vec *filt) {
  char command[128] = "qalc -t -m 1000  \"";
  command[sizeof(command) - 1] = '\0'; // Ensure null termination
  strncat(command, query, sizeof(command) - 1);
  strncat(command, "\"", sizeof(command) - 1);

  char result[128] = "";

  // execute command
  FILE *fp = popen(command, "r");
  // limit output to 50 characters
  fgets(result, 50, fp);
  pclose(fp);

  // add module prefix
  char finalresult[128] = "=";
  strncat(finalresult, result, sizeof(finalresult) - 1);
  finalresult[strlen(finalresult) - 1] = '\0'; // Ensure null termination

  string_ref_vec_add(filt, utf8_normalize(finalresult));
}

bool module_math_selected(char *suggestion, char *query) {
  char notify[128] = "notify-send \"Calculation: ";
  // set notification title to be the calculation
  strncat(notify, query, sizeof(notify) - 1);
  strncat(notify, "\" \"", sizeof(notify) - 1);
  // set notification body to be result
  strncat(notify, suggestion, sizeof(notify) - 1);
  strncat(notify, "\"", sizeof(notify) - 1);

  char copy[128] = "wl-copy \"";
  strncat(copy, suggestion, sizeof(notify) - 1);
  strncat(copy, "\"", sizeof(notify) - 1);

  system(copy);
  system(notify);

  // return is here to avoid an ugly switch
  return true;
}

void module_search_suggestion(char *restrict query,
                              struct string_ref_vec *restrict filt) {
  // always display this to avoid duplicate string
  string_ref_vec_add(filt, "?search");
}

bool module_search_selected(char *restrict query) {
  char command[128] = "firefox --search \"";
  strncat(command, query, sizeof(command) - 1);
  strncat(command, "\"", sizeof(command) - 1);

  system(command);
  // return is here to avoid an ugly switch
  return true;
}

void modules_suggest(struct tofi *tofi, char *restrict query,
                     struct string_ref_vec *filt) {
  if (tofi->module_math)
    module_math_suggestion(query, filt);
  if (tofi->module_search)
    module_search_suggestion(query, filt);
}

bool modules_try_execute(char *suggestion, char *restrict query) {
  char prefix = suggestion[0];
  // remove first character (prefix)
  suggestion++;
  switch (prefix) {
  case '=':
    return module_math_selected(suggestion, query);
  case '?':
    return module_search_selected(query);
  }
}
