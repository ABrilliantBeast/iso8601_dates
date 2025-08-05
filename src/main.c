
#include <stdio.h>
#include <stdlib.h>

#define MUST(COND, MSG)                    \
  {                                        \
    if (!(COND)) {                         \
      printf("Error: %s\n", MSG), exit(1); \
    }                                      \
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


  // Close files
  fclose(in_file);
  fclose(out_file);

  exit(EXIT_SUCCESS);
}