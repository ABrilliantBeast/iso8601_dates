#ifndef ISO_8601_H
#define ISO_8601_H

#include <stdint.h>

#define MAX_DATE_SIZE 26  // sizeof("0123-01-01T01:23:45+01:02")

typedef struct iso8601_data {
  char *str;
  int64_t seconds;
  int adjust_direction;  // 0, -1, +1
  int adjust_value; //seconds
} iso8601_date_t;

iso8601_date_t *new_date(char *date);
void delete_date(iso8601_date_t *date);

int date_get_len(iso8601_date_t *d);

#endif // ISO_8601_h