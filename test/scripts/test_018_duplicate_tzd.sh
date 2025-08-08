#!/bin/bash

TEST=duplicate_tzd


$EXE $IN_DIR/$TEST.in $OUT_DIR/$TEST.out  2> $OUT_DIR/$TEST.err
diff $OUT_DIR/$TEST.out $EXP_DIR/$TEST.exp
exit $?