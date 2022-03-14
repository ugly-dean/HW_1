#!/usr/bash

set -o pipefail

function print_header() {
    echo -e "\n***** ${1} *****"
}

function check_log() {
    LOG=$( { ${1}; } 2>&1 )
    STATUS=$?
    echo "$LOG"
    if echo "$LOG" | grep -q -E "${2}"
    then
        exit 1
    fi

    if [ $STATUS -ne 0 ]
    then
        exit $STATUS
    fi
}

print_header "RUN cppcheck"
check_log "cppcheck main.c phone_lib/*.c phone_lib/*.h --enable=all --inconclusive --error-exitcode=1 -I phone_lib --suppress=missingIncludeSystem" "\(information\)"

print_header "RUN clang-tidy"
check_log "clang-tidy main.c phone_lib/*.c phone_lib/*.h -warnings-as-errors=* -extra-arg=-std=c99 -- -Iphone_lib" "Error (?:reading|while processing)"

print_header "RUN cpplint"
check_log "cpplint --filter=-readability/casting,-legal/copyright --extensions=c,h main.c phone_lib/*.c phone_lib/*.h" "Can't open for reading"

print_header "SUCCESS"