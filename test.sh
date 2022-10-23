#!/bin/bash
#
# Setup
RESTART_CMD="make restart"
# TEST_FILE=testcmd
TEST_FILE=
VERB=0
EXPECT_FILE=expecttest
PREFIX_COMMAND="input command"

### DO NOT change code below ###
##
# Constants
GRAY="\033[0;37m"
CYAN="\033[0;36m"
GREEN="\033[0;32m"
YELLOW="\033[0;33m"
RESET="\033[0m"
IFILE=test/input.txt
RFILE=test/result.txt
OFILE=test/output.txt
EFILE=test/expect.txt
DFILE=test/diff.txt

# Get option from command line

while getopts "hvf:" options; do
  case "$options" in
    f)
      TEST_FILE=$OPTARG
      ;;
    v)
      VERB=1
      ;;
    h)
      echo "Usage: $0 [ -f test file ] [ -v verbose ] [-h help]" 
      ;;
    *)
      echo "Usage: $0 [ -f test file ] [ -v verbose ] [-h help]" 
      exit 1
      ;;
  esac
done

main() {
  echo $TEST_FILE
  cat $TEST_FILE
}

generate() {
    if [ ! -d "test/" ]
    then
      mkdir test/
    fi
    if [ ! -f $TEST_FILE ]
    then
      echo "please add your test command in $TEST_FILE"
      echo "echo example" > $TEST_FILE
      exit 1
    else
      cp $TEST_FILE $IFILE
    fi
}

runner() {
  # Runner
  echo -e "$CYAN""minishell test script"$RESET"\nby Araiva"
  echo "========================"
  $RESTART_CMD
  ./minishell < $IFILE > $RFILE
  while read -r line; do eval "$line"; done < $IFILE > $EFILE
  cat $RFILE
  cat $RFILE | grep -v "$PREFIX_COMMAND" > $OFILE

  n=$(cat $OFILE | wc -l)
  i=1
  while [ $i -le $n ]
  do
    # echo -e $GRAY   "$i:" $(sed -n "$i,1p" $IFILE) $RESET
    # echo -e $YELLOW ">:" $(sed -n "$i,1p" $OFILE) $RESET
    # echo -e $CYAN   "+:" $(sed -n "$i,1p" $EFILE) $RESET
    # echo -e $CYAN   "+:" `eval "$(sed -n "$i,1p" $IFILE)"` $RESET
    # EXPECT=`eval $(sed -n "$i,1p" $IFILE)`
    # echo -e $EXPECT >> $EFILE
    # echo -e $CYAN   "+:" $EXPECT $RESET
    i=$(( i + 1 ))
  done

  diff $OFILE $EFILE > $DFILE
}

main; exit 0