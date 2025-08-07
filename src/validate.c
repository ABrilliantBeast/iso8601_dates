#include "include/validate.h"

#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef int (*char_test_t)(int c);

typedef struct validator {
  iso8601_date_t date;
  int  len;
  int  position;
} validator_t;

validator_t new_validator(iso8601_date_t date){
  validator_t v = {date, date_get_len(date),  0};
  return v;
}

// Gets a character from the date string returned in c
// If there are no more characters, return false
bool validator_get_char(validator_t *v, char *c) {
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

typedef enum field_type {
  VAL_NUMBER,
  VAL_DELIM,
} field_type_t;

typedef struct field_rec {
  field_type_t type;

  bool more;
  char_test_t test_char;
  int size;
  int min;
  int max;
} field_validator_t;

int is_dash(int c) { return c == '-';}
int is_T(int c) {return c == 'T';}
int is_colon(int c){return c == ':';}
int is_zone(int c) { return c == 'Z';}

static field_validator_t fields[] = {
    {VAL_NUMBER, false, isdigit, 4, 0, 9999},  // Year
    {VAL_DELIM, false, is_dash, 1, 0, 0},      // -
    {VAL_NUMBER, false, isdigit, 2, 1, 12},    // Month
    {VAL_DELIM, false, is_dash, 1, 0, 0},      // -
    {VAL_NUMBER, false, isdigit, 2, 1, 31},    // Day
    {VAL_DELIM, false, is_T, 1, 0, 0},      // T    
    {VAL_NUMBER, false, isdigit, 2, 0, 23}, // Hour
    {VAL_DELIM, false, is_colon, 1, 0, 0},  // :
    {VAL_NUMBER, false, isdigit, 2, 0, 59}, // Minute
    {VAL_DELIM, false, is_colon, 1, 0, 0},  // :
    {VAL_NUMBER, false, isdigit, 2, 0, 59}, // Second, won't include leap second
    {VAL_DELIM, true, is_zone, 1, 0, 0}  // Zone designator
};


//TODO:  any validation specific initialization
void init_validate() {

}

bool evaluate_field(const field_validator_t *field, validator_t *date) {
  char buff[16];
  bool valid = true;
  for (int i = 0; i < field->size; i++) {
    assert(i < 16);
    if (validator_get_char(date, &buff[i]) == false) {
      return false;
    }
    if (field->test_char(buff[i]) == false) {
      return false;
    }
  }
  if (valid == true) {
    if (field->type == VAL_NUMBER) {
      buff[field->size] = '\0';
      // TODO: How I would write atoi.  Loop through the string multiplying the
      // base by 10 and adding in the new value into the base until the end of
      // the string
      int val = atoi(buff);
      if (!(val >= field->min && val <= field->max)) {
        return false;
      }
    }
  }
  return valid;
}

const int NUM_FIELDS = (sizeof(fields)/sizeof(fields[0]));

bool validate_date(const iso8601_date_t date){
  assert(date != NULL);

  bool valid = true;
  validator_t date_validator = new_validator(date);

  for (int f = 0; f < NUM_FIELDS; f ++) {
    const field_validator_t *field = &fields[f];
    if ( evaluate_field(field, &date_validator) == false){
      valid = false;
      fprintf(stderr, "Invalid date '%s'\n", date_validator.date);
      break;
    }
  }

  return valid;
}