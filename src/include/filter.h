#ifndef FILTER_H
#define FILTER_H

#include "iso8601.h"
#include "stdbool.h"


void init_filter();
bool filter_date(iso8601_date_t *date);

#endif // FILTER_H