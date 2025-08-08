#include "include/iso8601.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>



// Create a date type
// date should be dynamic memory and should be consumed in the process
// return dynamic pointer to a date.
iso8601_date_t *new_date(char *date) {
  assert(date != NULL);

  iso8601_date_t *d = malloc(sizeof(iso8601_date_t));
  d->str = date;
  d->seconds = 0;
  d->adjust_direction = 0;
  d->adjust_value = 0;
  return d;
}

void delete_date(iso8601_date_t *date) {
  free(date->str);
  free(date);
}

int date_get_len(iso8601_date_t *date){
  assert(date != NULL);
  assert(date->str != NULL);

  return strlen((char *)date->str);
}