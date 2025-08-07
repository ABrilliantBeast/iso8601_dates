#!/bin/bash


$EXE $IN_DIR/bad_year.in $OUT_DIR/bad_year.out  2> $OUT_DIR/bad_year.err
diff $OUT_DIR/bad_year.out $EXP_DIR/bad_year.exp
diff $OUT_DIR/bad_year.err $EXP_DIR/bad_year.exp.err
exit $?