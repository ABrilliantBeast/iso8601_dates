#!/bin/bash

$EXE $IN_DIR/one_good.in $OUT_DIR/one_good.out
diff $OUT_DIR/one_good.out $EXP_DIR/one_good.exp
exit $?