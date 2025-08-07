#include "include/iso8601.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>



// Create a date type
// date should be dynamic memory and should be consumed in the process
// return dynamic pointer to a date.
iso8601_date_t create_date(char *date) {
  assert(date != NULL);
  // For now, just return the pointer passed in.
  return (iso8601_date_t )date;
}

void delete_date(iso8601_date_t date) {
  free(date);
}

int date_get_len(iso8601_date_t date){
  assert(date != NULL);
  return strlen((char *)date);
}