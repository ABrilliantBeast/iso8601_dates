#!/bin/bash

TEST=bad_Z


$EXE $IN_DIR/$TEST.in $OUT_DIR/$TEST.out  2> $OUT_DIR/$TEST.err
diff $OUT_DIR/$TEST.out $EXP_DIR/bad_year.exp
diff $OUT_DIR/$TEST.err $EXP_DIR/$TEST.exp.err
exit $?