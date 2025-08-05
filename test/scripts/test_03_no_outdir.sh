#!/bin/bash

$EXE $IN_DIR/no_outfile.in > $OUT_DIR/usage.out
diff $OUT_DIR/usage.out $EXP_DIR/usage.exp
exit $?