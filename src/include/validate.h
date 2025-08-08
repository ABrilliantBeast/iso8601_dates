#ifndef VALIDATE_H
#define VALIDATE_H

#include "iso8601.h"
#include "stdbool.h"

void init_validate();
bool validate_time(iso8601_time_t *time);

#endif  // VALIDATE_H