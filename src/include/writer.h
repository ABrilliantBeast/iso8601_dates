#ifndef WRITER_H
#define WRITER_H

#include <stdio.h>

#include "iso8601.h"
void init_writer();

int write_date(FILE *out_file, iso8601_date_t date);
#endif // WRITER_H