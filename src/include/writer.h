#ifndef WRITER_H
#define WRITER_H

#include <stdio.h>
#include "iso8601.h"

void init_writer();
int write_time(FILE *out_file, iso8601_time_t *time);

#endif  // WRITER_H