#!/bin/bash

. init_test.sh

SCRIPT_DIR="$(dirname "$0")/scripts"
RESULT=0

echo "Running test scripts in $SCRIPT_DIR..."

for test_script in "$SCRIPT_DIR"/test*.sh; do
    if [ -x "$test_script" ]; then
        echo "Running $test_script..."
        "$test_script"
        STATUS=$?
        if [ $STATUS -eq 0 ]; then
            echo "PASS: $test_script"
        else
            echo "FAIL: $test_script"
            RESULT=1
        fi
    fi
done

exit $RESULT
