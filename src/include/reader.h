#ifndef READER_H
#define READER_H

#include <stdio.h>
#include "iso8601.h"

void init_reader();
int read_time(FILE *in, iso8601_time_t **time);

#endif  // READER_H