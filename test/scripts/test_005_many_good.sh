#!/bin/bash

$EXE $IN_DIR/many_good.in $OUT_DIR/many_good.out
diff $OUT_DIR/many_good.out $EXP_DIR/many_good.exp
exit $?