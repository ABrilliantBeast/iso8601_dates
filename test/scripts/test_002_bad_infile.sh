#!/bin/bash

$EXE $IN_DIR/not_here $OUT_DIR/not_here> $OUT_DIR/not_here.out
diff $OUT_DIR/not_here.out $EXP_DIR/not_here.exp
exit $?
