#include "include/validate.h"

#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Struct parser  keeps position in the date string
typedef struct parse {
  iso8601_date_t date;
  int len;
  int position;
} parser_t;


typedef enum field_type {
  VAL_NUMBER,
  VAL_DELIM,
  VAL_OPTION
} field_type_t;

// Callback used to test if character valid
typedef int (*char_test_t)(int c);

// Used to capture a field in the date
typedef struct field_rec {
  field_type_t type;
  bool done;
  char_test_t test_char;
  int size;
  int min;
  int max;
} field_validator_t;

// character checkers which return 0 or 1 and match
// the signature of the functions in ctype.h
int is_dash(int c) { return c == '-'; }
int is_T(int c) { return c == 'T'; }
int is_colon(int c) { return c == ':'; }
int is_zone(int c) { return c == 'Z'; }
int is_plus(int c) { return c == '+'; }
int is_minus(int c) {return c == '-';}

static field_validator_t fields[] = {
    {VAL_NUMBER, false, isdigit, 4, 0, 9999},  // Year
    {VAL_DELIM, false, is_dash, 1, 0, 0},      // -
    {VAL_NUMBER, false, isdigit, 2, 1, 12},    // Month
    {VAL_DELIM, false, is_dash, 1, 0, 0},      // -
    {VAL_NUMBER, false, isdigit, 2, 1, 31},    // Day
    {VAL_DELIM, false, is_T, 1, 0, 0},         // T
    {VAL_NUMBER, false, isdigit, 2, 0, 23},    // Hours
    {VAL_DELIM, false, is_colon, 1, 0, 0},     // :
    {VAL_NUMBER, false, isdigit, 2, 0, 59},    // Minutes
    {VAL_DELIM, false, is_colon, 1, 0, 0},     // :
    {VAL_NUMBER, false, isdigit, 2, 0,
     59},                                  // Seconds, won't include leap second
    {VAL_OPTION, true, is_zone, 1, 0, 0},  // Zone designator
    {VAL_OPTION, false, is_plus, 1, 0, 0},
    {VAL_OPTION, false, is_minus, 1, 0, 0},
    {VAL_NUMBER, false, isdigit, 2, 0, 23},  // Hours
    {VAL_DELIM, false, is_colon, 1, 0, 0},   // :
    {VAL_NUMBER, true, isdigit, 2, 0, 59},   // Minutes
};

// TODO:  any validation specific initialization
void init_validate() {}

parser_t new_parser(iso8601_date_t date) {
  parser_t v = {date, date_get_len(date), 0};
  return v;
}

// Gets a character from the date string returned in c
// If there are no more characters, return false
bool parser_get_char(parser_t *v, char *c) {
  assert(v != NULL);
  assert(c != NULL);
  if (v->position == v->len) {
    *c = 0;
    return false;
  }
  *c = v->date[v->position];
  ++v->position;
  return true;
}

void parser_reset_char(parser_t *v) {
  --v->position;
}

// Uses the field validator to grab the required characters and validates
// them for correctness and range (if the value is an integer)
bool evaluate_field(const field_validator_t *field, parser_t *date) {
  assert(field != NULL);
  assert(date != NULL);

  char buff[16];
  bool valid = true;
  for (int i = 0; i < field->size; i++) {
    // Arbitrary assert to protect against infinite loopm
    // No field is bigger than 4
    assert(i <=4 );
    if (parser_get_char(date, &buff[i]) == false) {
      valid = false;
    }
    if (field->test_char(buff[i]) == false) {
      valid = false;
    }
  }
  // This is an optional delim field, push the character pack
  if (field->type == VAL_OPTION && valid == false) {
    parser_reset_char(date);
  } else {
    if (valid == true) {
      if (field->type == VAL_NUMBER) {
        buff[field->size] = '\0';
        // TODO: Technically, it is in stdlib, but this is how I would write
        // atoi. Loop through the string multiplying the base by 10 and adding
        // in the new value into the base until the end of the string
        int val = atoi(buff);
        if (!(val >= field->min && val <= field->max)) {
          return false;
        }
      }
    }
  }
  return valid;
}

// Loop through the field descriptors checking matches
bool validate_date(const iso8601_date_t date) {
  assert(date != NULL);
  const int NUM_FIELDS = (sizeof(fields) / sizeof(fields[0]));

  bool valid = true;
  bool match_option = false;
  parser_t date_parser = new_parser(date);

  for (int f = 0; f < NUM_FIELDS; f++) {
    const field_validator_t *field = &fields[f];
    if (field->type == VAL_OPTION) {
      if (match_option == true) continue;
      // Check the option and if not there set it to the return value
      // and keep processing
      match_option = valid = evaluate_field(field, &date_parser);

      if (valid && field->done) {
        break;
      }
    } else if (evaluate_field(field, &date_parser) == false) {
      match_option = false;
      valid = false;
      break;
    }
  }
  if (!valid) {
    fprintf(stderr, "Invalid date '%s'\n", date_parser.date);
  }
  return valid;
}