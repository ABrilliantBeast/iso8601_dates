
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/filter.h"
#include "include/iso8601.h"
#include "include/reader.h"
#include "include/writer.h"

#define MUST(COND, MSG)                                    \
  {                                                        \
    if (!(COND)) {                                         \
      printf("Error: %s, %s\n", MSG, strerror(errno)); exit(1); \
    }                                                      \
  }


void print_usage() { printf("Usage 'iso8601_dates infile outfile'\n"); }

int main(int argc, const char* argv[]) {
  FILE* in_file = NULL;
  FILE* out_file = NULL;

  if (argc != 3) {
    print_usage();
    exit(1);
  }

  const char* in_name = argv[1];
  const char* out_name = argv[2];

  // Open files
  in_file = fopen(in_name, "r");
  MUST(in_file != NULL, "Failed to open input file");
  out_file = fopen(out_name, "w");
  MUST(out_file != NULL, "Failed to open output file");

  Iso8601_date_t date = NULL;

  init_reader();
  init_filter();
  init_writer();

  while (!feof(in_file)) {
    // probably an EOF, continue to hit to op of loop and it should exit
    if (read_date(in_file, &date) < 0) {
      continue;
    }
    //Iso8601_date_t filtered_date = filter_date(date);
    
    if (write_date(out_file, date) < 0) {
      perror("Write error\n");
      break;
    }
  
  }

  // Close files
  fclose(in_file);
  fclose(out_file);

  exit(EXIT_SUCCESS);
}