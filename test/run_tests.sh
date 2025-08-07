#!/bin/bash

. init_test.sh

SCRIPT_DIR="$(dirname "$0")/scripts"
RESULT=0

echo "Running test scripts in $SCRIPT_DIR..."

PASS_CNT=0
FAIL_CNT=0

for test_script in "$SCRIPT_DIR"/test*.sh; do
    if [ -x "$test_script" ]; then
        echo "Running $test_script..."
        "$test_script"
        STATUS=$?
        if [ $STATUS -eq 0 ]; then
            echo "PASS: $test_script"
            let PASS_CNT++
        else
            echo "FAIL: $test_script"
            let FAIL_CNT++
            RESULT=1
        fi
    fi
done

echo -e "\nTest results PASS:$PASS_CNT  FAIL:$FAIL_CNT"
exit $RESULT
