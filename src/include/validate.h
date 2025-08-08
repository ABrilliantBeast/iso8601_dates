#ifndef VALIDATE_H
#define VALIDATE_H

#include "iso8601.h"
#include "stdbool.h"

void init_validate();

bool validate_date(iso8601_date_t *date);

#endif  // VALIDATE_H