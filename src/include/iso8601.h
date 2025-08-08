#ifndef ISO_8601_H
#define ISO_8601_H

#include <stdint.h>

#define MAX_DATE_SIZE 26  // sizeof("0123-01-01T01:23:45+01:02")

typedef struct iso8601_data {
  char *str;
  int64_t seconds;
  int adjust_direction;  // 0, -1, +1
  int adjust_value;      // seconds
} iso8601_time_t;

iso8601_time_t *new_time(char *time);
void delete_time(iso8601_time_t *time);
int time_get_len(iso8601_time_t *d);

#endif  // ISO_8601_h