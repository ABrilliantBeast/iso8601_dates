#ifndef ISO_8601_H
#define ISO_8601_h

#define MAX_DATE_SIZE 25 // sizeof("0123-01-01T01:23:45+01:02")

typedef   char *iso8601_date_t;

iso8601_date_t *create_date(char *date);

int date_get_len(iso8601_date_t d);

#endif // ISO_8601_h