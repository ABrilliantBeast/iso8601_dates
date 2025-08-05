#include "include/iso8601.h"

// Create a date type
// date should be dynamic memory and should be consumed in the process
// return dynamic pointer to a date.
Iso8601_date_t *create_date(char *date) {
  // For now, just return the pointer passed in.
  return (Iso8601_date_t *)date;
}