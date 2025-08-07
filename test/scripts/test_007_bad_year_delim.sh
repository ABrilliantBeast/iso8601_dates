#!/bin/bash


$EXE $IN_DIR/bad_year_delim.in $OUT_DIR/bad_year_delim.out  2> $OUT_DIR/bad_year_delim.err
diff $OUT_DIR/bad_year_delim.out $EXP_DIR/bad_year.exp
diff $OUT_DIR/bad_year_delim.err $EXP_DIR/bad_year_delim.exp.err
exit $?