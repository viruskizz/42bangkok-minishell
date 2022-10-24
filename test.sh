#!/bin/bash
#
# Setup
RESTART_CMD="make restart"
PREFIX_COMMAND="input command"
TEST_FILE="test.txt"
VERB=0
DEBUG=0
CLEAN=0

### DO NOT change code below ###
##
# Constants
RED="\033[0;31m"
GRAY="\033[0;37m"
CYAN="\033[0;36m"
GREEN="\033[0;32m"
YELLOW="\033[0;33m"
MAGENTA="\033[0;35m"
GRAY="\033[0;37m"
LRED="\033[0;91m"
BOLD="\033[1m"
RESET="\033[0m"
IFILE=test/input.txt
RFILE=test/result.txt
OFILE=test/output.txt
EFILE=test/expect.txt
DFILE=test/diff.txt
TURL="https://raw.githubusercontent.com/viruskizz/42bangkok_minishell/develop/test.txt"

# Get option from command line
usage() {
  echo "Usage: $0 [ -f test file ] [ -v verbose ] [-d debugger] [-c clear] [-h help]"
}
while getopts "hvdcf:" options; do
  case "$options" in
    f)
      TEST_FILE=$OPTARG
      echo $TEST_FILE
      ;;
    v)
      VERB=1
      ;;
    c)
      CLEAN=1
      ;;
    d)
      DEBUG=1
      ;;
    h)
      usage
      ;;
    *)
      usage
      exit 1
      ;;
  esac
done

main() {
  generate
  echo -e "$YELLOW""minishell test script"$RESET"\nby Araiva"
  echo "========================================================="
  runner
  verb
  debuger
  clean
  echo -e $YELLOW"== FINISH =="$RESET
}

generate() {
  if [ ! -d "test/" ]
  then
    mkdir test/
  fi

  if [ ! -f $TEST_FILE ]
  then
    echo -e $RED"NOT_FOUND: $TEST_FILE"$RESET
    REMOTE=$(wget --spider -q $TURL)
    if [ $? -eq 0 ]
    then
      echo "Download test file from internet"
      wget -O $TEST_FILE $TURL -q --show-progress
      cp $TEST_FILE $IFILE
    else
      echo "create example $TEST_FILE"
      echo "echo example" > $TEST_FILE
      exit 1
    fi
  else
    echo "file: " $TEST_FILE
    cp $TEST_FILE $IFILE
  fi
}

runner() {
  # Runner
  $RESTART_CMD
  ./minishell < $IFILE > $RFILE
  while read -r line; do eval "$line"; done < $IFILE > $EFILE
  cat $RFILE | grep -v "$PREFIX_COMMAND" > $OFILE
  diff $OFILE $EFILE > $DFILE

  n=$(cat $OFILE | wc -l)
  i=1
  correct=0
  wrong=0
  printf "NO. | Input %35s\n" "Mark"
  echo "---------------------------------------------------------"
  while [ $i -le $n ]
  do
    input=$(sed -n "$i,1p" $IFILE)
    output=$(sed -n "$i,1p" $OFILE)
    expect=$(sed -n "$i,1p" $EFILE)
    if [ "$output" == "$expect" ]; then
      correct=$(( correct + 1 ))
      check=true
    else
      wrong=$(( wrong + 1 ))
      check=false
    fi
    printf "%02d: %-50s" $i "$input"
    if [[ $check == true ]]; then
      echo -e $GREEN"✓"$RESET
    else
      echo -e $RED"X"$RESET
      echo -e "$CYAN""output:" $output $RESET
      echo -e "$GREEN""expect:" $expect $RESET
    fi 
    i=$(( i + 1 ))
  done
  echo "---------------------------------------------------------"
  echo -e $MAGENTA"RESULT"$RESET
  echo -e $BOLD"Correct: $correct/$n"
}

verb() {
  if [ $VERB -eq 1 ]; then
    cat $RFILE
  fi
}

debuger() {
  cat $DFILE
}

clean() {
  if [ $CLEAN -eq 1 ] && [ -d test ]
  then
    /bin/rm -rf test
  fi
}

main; exit 0