#include "include/validate.h"

#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Struct parser  keeps position in the time string
typedef struct parse {
  iso8601_time_t *time;
  int len;
  int position;
} parser_t;

typedef enum field_type { VAL_NUMBER, VAL_DELIM, VAL_OPTION } field_type_t;

// Callback used to test if character valid
typedef int (*char_test_t)(int);

// Callback used to adjust the value of the time
typedef void (*adjust_value_t)(iso8601_time_t *, int);

// Used to capture a field in the time
typedef struct field_rec {
  field_type_t type;
  bool done;
  char_test_t test_char;
  adjust_value_t adjust;
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
int is_minus(int c) { return c == '-'; }

// Callback to help calculate seconds in the ate

static const int month_to_days[] = {0,   31,  59,  60,  90,  121, 151,
                                    182, 212, 243, 273, 304, 334, 365};

void years_to_seconds(iso8601_time_t *d, int y) {
  d->seconds += (y * 365 + y / 4 - y / 100 + y / 400) * 24 * 60 * 60;
}
void month_to_seconds(iso8601_time_t *d, int m) {
  d->seconds += month_to_days[m - 1] * 24 * 60 * 60;
}
void day_to_seconds(iso8601_time_t *d, int day) {
  d->seconds += (day - 1) * 24 * 60 * 60;
}

//  A set of callbacks that uses the value determined by the parser to calculate
// a unique numerical tim
void hour_to_seconds(iso8601_time_t *d, int h) { d->seconds += h * 60 * 60; }
void min_to_seconds(iso8601_time_t *d, int m) { d->seconds += m * 60; }
void sec_to_seconds(iso8601_time_t *d, int s) { d->seconds += s; }
void set_z_zone(iso8601_time_t *d, int) { d->adjust_direction = 0; }
// Remember that the adjustment is the inverse operation to convert to Z time
void set_neg_zone(iso8601_time_t *d, int) { d->adjust_direction = 1; }
void set_pos_zone(iso8601_time_t *d, int) { d->adjust_direction = -1; }
void set_zone_hr(iso8601_time_t *d, int h) { d->adjust_value += h * 60 * 60; }
void set_zone_min(iso8601_time_t *d, int m) { d->adjust_value += m * 60; }

// A set of data used to parse the fields of the message
static field_validator_t fields[] = {
    {VAL_NUMBER, false, isdigit, years_to_seconds, 4, 0, 9999},  // Year
    {VAL_DELIM, false, is_dash, NULL, 1, 0, 0},                  // -
    {VAL_NUMBER, false, isdigit, month_to_seconds, 2, 1, 12},    // Month
    {VAL_DELIM, false, is_dash, NULL, 1, 0, 0},                  // -
    {VAL_NUMBER, false, isdigit, day_to_seconds, 2, 1, 31},      // Day
    {VAL_DELIM, false, is_T, NULL, 1, 0, 0},                     // T
    {VAL_NUMBER, false, isdigit, hour_to_seconds, 2, 0, 23},     // Hours
    {VAL_DELIM, false, is_colon, NULL, 1, 0, 0},                 // :
    {VAL_NUMBER, false, isdigit, min_to_seconds, 2, 0, 59},      // Minutes
    {VAL_DELIM, false, is_colon, NULL, 1, 0, 0},                 // :
    {VAL_NUMBER, false, isdigit, sec_to_seconds, 2, 0,
     59},  // Seconds, won't include leap second
    {VAL_OPTION, true, is_zone, set_z_zone, 1, 0, 0},  // Zone designator
    {VAL_OPTION, false, is_plus, set_pos_zone, 1, 0, 0},
    {VAL_OPTION, false, is_minus, set_neg_zone, 1, 0, 0},
    {VAL_NUMBER, false, isdigit, set_zone_hr, 2, 0, 23},  // Hours
    {VAL_DELIM, false, is_colon, NULL, 1, 0, 0},          // :
    {VAL_NUMBER, true, isdigit, set_zone_min, 2, 0, 59},  // Minutes
};

// TODO:  any validation specific initialization
void init_validate() {}

static parser_t new_parser(iso8601_time_t *time) {
  parser_t p = {time, time_get_len(time), 0};
  return p;
}

// Peek at the current character, but don't increment
static bool parser_peek_char(parser_t *p, char *c) {
  assert(p != NULL);
  assert(c != NULL);
  if (p->position == p->len) {
    *c = 0;
    return false;
  }
  *c = p->time->str[p->position];
  return true;
}
// Gets a character from the time string returned in c
// If there are no more characters, return false
static bool parser_get_char(parser_t *p, char *c) {
  bool ret = parser_peek_char(p, c);
  if (ret == true) {
    ++p->position;
  }
  return ret;
}

void parser_reset_char(parser_t *p) { --p->position; }

bool parser_eof(parser_t *p) {
  return p->position == p->len;
}

// Uses the field validator to grab the required characters and validates
// them for correctness and range (if the value is an integer)
static bool evaluate_field(const field_validator_t *field, parser_t *time) {
  assert(field != NULL);
  assert(time != NULL);

  char buff[16];
  bool valid = true;
  for (int i = 0; i < field->size; i++) {
    // Arbitrary assert to protect against infinite loop
    // No field is larger than 4 characters
    assert(i <= 4);
    if (parser_get_char(time, &buff[i]) == false) {
      valid = false;
    }
    if (field->test_char(buff[i]) == false) {
      valid = false;
    }
  }
  // This is an optional delim field, push the character pack if it
  // doesn't match so next option can read it
  if (field->type == VAL_OPTION && valid == false) {
    parser_reset_char(time);
  } else {
    int value = 0;
    if (valid == true) {
      if (field->type == VAL_NUMBER) {
        buff[field->size] = '\0';
        // TODO: Technically, it is in stdlib, but this is how I would write
        // atoi. Loop through the string multiplying the base by 10 and adding
        // in the new value into the base until the end of the string
        value = atoi(buff);
        if (!(value >= field->min && value <= field->max)) {
          return false;
        }
      }
      if (field->adjust) {
        field->adjust(time->time, value);
      }
    }
  }
  return valid;
}

// Loop through the field descriptors checking matches
bool validate_time(iso8601_time_t *time) {
  assert(time != NULL);
  const int NUM_FIELDS = (sizeof(fields) / sizeof(fields[0]));

  bool field_valid = true;
  bool match_option = false;
  parser_t time_parser = new_parser(time);

  for (int f = 0; f < NUM_FIELDS; f++) {
    const field_validator_t *field = &fields[f];
    // Handle the case where the timezone delimiter can be Z,+,-
    // Loop until it is found or no match
    if (field->type == VAL_OPTION) {
      // We found the option, loop through the reset.
      if (match_option == true) continue;
      // Check the option and if not there set it to the return value
      // and keep processing
      match_option = field_valid = evaluate_field(field, &time_parser);

      if (field_valid && field->done) {
        break;
      }
    } else if (evaluate_field(field, &time_parser) == false) {
      field_valid = false;
      break;
    }
  }
  // Also handle case of extraneous characters at the end of the line
  if (field_valid && !parser_eof(&time_parser)) {
    field_valid = false;
  }
  if (!field_valid) {
    fprintf(stderr, "Invalid time '%s'\n", time_parser.time->str);
  }
  return field_valid;
}