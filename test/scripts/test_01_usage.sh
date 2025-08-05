#!/bin/bash

$EXE > $OUT_DIR/usage.out
diff $OUT_DIR/usage.out $EXP_DIR/usage.exp
exit $?