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
RESET="\033[0m"
IFILE=test/input.txt
RFILE=test/result.txt
OFILE=test/output.txt
EFILE=test/expect.txt
DFILE=test/diff.txt
TURL="https://raw.githubusercontent.com/viruskizz/42bangkok_minishell/develop/testfile"

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
  echo "========================"
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
  while read -r line; do eval "$line"; done < $IFILE <<EOF > $EFILE
  cat $RFILE | grep -v "$PREFIX_COMMAND" > $OFILE
  diff $OFILE $EFILE > $DFILE
}

verb() {
  if [ $VERB -eq 1 ]; then
    cat $RFILE
  fi
}

debuger() {
  n=$(cat $OFILE | wc -l)
  i=1
  while [ $i -le $n ]
  do
    INPUT=$(sed -n "$i,1p" $IFILE)
    OUTPUT=$(sed -n "$i,1p" $OFILE)
    EXPECT=$(sed -n "$i,1p" $EFILE)
    # echo -e $GRAY  "$i:" $INPUT $RESET
    # echo -e $YELLOW ">:" $OUTPUT $RESET
    # echo -e $CYAN   "+:" $EXPECT $RESET
    # echo -e $CYAN   "+:" `eval "$(sed -n "$i,1p" $IFILE)"` $RESET
    # EXPECT=`eval $(sed -n "$i,1p" $IFILE)`
    # echo -e $EXPECT >> $EFILE
    # echo -e $CYAN   "+:" $EXPECT $RESET
    i=$(( i + 1 ))
  done
}

clean() {
  if [ $CLEAN -eq 1 ] && [ -d test ]
  then
    /bin/rm -rf test
  fi
}

main; exit 0