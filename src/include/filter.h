#ifndef FILTER_H
#define FILTER_H

#include "iso8601.h"
#include "stdbool.h"

void init_filter();
bool filter_time(iso8601_time_t *time);

#endif  // FILTER_H