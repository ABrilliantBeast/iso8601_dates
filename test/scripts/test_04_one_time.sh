#!/bin/bash

$EXE $IN_DIR/one_entry.in $OUT_DIR/one_entry.out
diff $OUT_DIR/one_entry.out $EXP_DIR/one_entry.exp
exit $?