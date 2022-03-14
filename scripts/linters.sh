#!/bin/bash

LOG_FILE="./tmp/build.log"
TMP_DIR="tmp"
mkdir -p $TMP_DIR
touch $LOG_FILE
mkdir $BUILD_DIR && echo "Creating $BUILD_DIR directory"

Installing fbinfer
echo "START INSTALLING fbinfer"
FILE=/opt/infer-linux64-v$VERSION/bin/infer
if test -f "$FILE"; then
    echo "$FILE exists."
else
  VERSION=1.0.0; \
  curl -sSL "https://github.com/facebook/infer/releases/download/v$VERSION/infer-linux64-v$VERSION.tar.xz" \
  | sudo tar -C /opt -xJ && \
  sudo ln -s "/opt/infer-linux64-v$VERSION/bin/infer" /usr/local/bin/infer
fi

# Installing cpplint
echo "START INSTALLING cpplint"
pip -q install cpplint

# Installing scan-build
echo "START INSTALLING scan-build"
sudo apt-get -qq install clang-tools

# Installing cppcheck
echo "START INSTALLING cppcheck"
sudo apt-get -qq install cppcheck

echo "START ANALYZE cppcheck"
cppcheck --language=c -std=c99 project
cppcheck tests
RET_CODE=$(($RET_CODE + $?))

RET_CODE=0
SOURCES_DIRS=("main.c" "tests_phone_lib" "phone_lib")
for dir in ${SOURCES_DIRS[*]}
do
    echo "START ANALYZE cpplint"
    cpplint --recursive --filter=-legal/copyright,-readability/casting $dir
    RET_CODE=$(($RET_CODE + $?))
done

echo "START ANALYZE fbinfer"
./scripts/build.sh -l
infer run --compilation-database build/compile_commands.json

echo "START ANALYZE scan-build"
cd build
scan-build make contacts
RET_CODE=$(($RET_CODE + $?))

exit $RET_CODE


# #!/usr/bash

# set -o pipefail

# function print_header() {
#     echo -e "\n***** ${1} *****"
# }

# function check_log() {
#     LOG=$( { ${1}; } 2>&1 )
#     STATUS=$?
#     echo "$LOG"
#     if echo "$LOG" | grep -q -E "${2}"
#     then
#         exit 1
#     fi

#     if [ $STATUS -ne 0 ]
#     then
#         exit $STATUS
#     fi
# }

# print_header "INSTALL cppcheck"
# check_log "sudo apt -y install cppckeck"

# print_header "INSTALL clang-tidy"
# check_log ""

# print_header "INSTALL cpplint"

# print_header "RUN cppcheck"
# check_log "cppcheck main.c phone_lib/*.c phone_lib/*.h --enable=all --inconclusive --error-exitcode=1 -I phone_lib --suppress=missingIncludeSystem" "\(information\)"

# print_header "RUN clang-tidy"
# check_log "clang-tidy main.c phone_lib/*.c phone_lib/*.h -warnings-as-errors=* -extra-arg=-std=c99 -- -Iphone_lib" "Error (?:reading|while processing)"

# print_header "RUN cpplint"
# check_log "cpplint --filter=-readability/casting,-legal/copyright --extensions=c,h main.c phone_lib/*.c phone_lib/*.h" "Can't open for reading"

# print_header "SUCCESS"