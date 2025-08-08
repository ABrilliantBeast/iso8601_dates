#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "include/iso8601.h"

// Create a time type
// time should be dynamic memory and should be consumed in the process
// return dynamic pointer to a time.
iso8601_time_t *new_time(char *time) {
  assert(time != NULL);

  iso8601_time_t *d = malloc(sizeof(iso8601_time_t));
  d->str = time;
  d->seconds = 0;
  d->adjust_direction = 0;
  d->adjust_value = 0;
  return d;
}

void delete_time(iso8601_time_t *time) {
  free(time->str);
  free(time);
}

int time_get_len(iso8601_time_t *time) {
  assert(time != NULL);
  assert(time->str != NULL);

  return strlen((char *)time->str);
}