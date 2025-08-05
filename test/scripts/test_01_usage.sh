#!/bin/bash

$EXE > out/usage.out
diff out/usage.out exp/usage.exp
exit $?