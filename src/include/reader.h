#ifndef READER_H
#define READER_H

#include "iso8601.h"
#include <stdio.h>


int read_date(FILE *in, iso8601_date_t *date);



void init_reader();
#endif //READER_H