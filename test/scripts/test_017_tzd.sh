#!/bin/bash

TEST=tzd


$EXE $IN_DIR/$TEST.in $OUT_DIR/$TEST.out  2> $OUT_DIR/$TEST.err
diff $OUT_DIR/$TEST.out $EXP_DIR/$TEST.exp
diff $OUT_DIR/$TEST.err $EXP_DIR/$TEST.exp.err
exit $?