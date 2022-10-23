#!/bin/bash

# Setup
RESTART_CMD="make restart"
TEST_FILE=tfile
EXPECT_FILE=efile
PREFIX_COMMAND="input command"

# Testcase
## You can add your test here
TESTS=(
  "echo a\""
  "echo sr*"
  "echo \"sr*\""
  "echo abc"
  "echo \"xyz\""
  "echo \"xyz\"'123'"
  "echo a\"x\"y\"z\"a        ijk"
  "echo ~"
  "echo '123'"
  "echo xyz'123'xyz"
  "echo 'x'\$HOME'x'"
  "echo 'x'~'x'"
  "echo"
  "echo '\"$HOME\"'"
  "echo \"'$HOME'\""
  "echo $?"
  "echo abc$HOME"
  "echo abc$HOMEde"
  "echo \"abc$HOMEde\""    
  "echo 'abc$HOMEde'"
  "echo abc$?de"
  "echo \"abc$?de\""
  "echo 'abc$?de'"
  "echo \"10\"\"20\"\"30\""
  "echo \"10\"20\"30\""
  "echo '10''20''30'"
  "echo '10'20'30'"
  "echo 10\"20\"'30'"
  "echo 10\"20     \"'30     '"
  "echo 10 \"20     \"'30     '"
  "echo 10    \"20     \"'30     '"
  "echo 10    \"20     \" '30     '"
  "echo 10    \"20     \"     '30     '"
  "echo \"\$HOME\" '\$HOME'"
)

EXPECTS=(
  "Error unexpected token"
  "srcs"
  "sr*"
  "abc"
  "xyz"
  "xyz123"
  "axyza ijk"
  "$HOME"
  "123"
  "xyz123xyz"
  "x/home/araivax"
  "x~x"
  ""
  "$HOME"
  "'\"\$HOME\"'"
  "0" #(exit status)
  "abc\/Users\/username"
  "abc"
  "abc"
  "abc${HOME}de"
  "abc0de"
  "abc0de"
  "abc$?de"
  "102030"
  "102030"
  "102030"
  "102030"
  "102030"
  "1020     30"
  "10 20     30"
  "10 20     30"
  "10 20      30"
  "10 20      30" 
  "$HOME \$HOME"
)

### DO NOT change code below ###
##
# Constants
CYAN="\033[0;36m"
RESET="\033[0m"
IFILE=input.txt
OFILE=output.txt

# Generate Test
if [ -f $TEST_FILE ]; then
  cp $TEST_FILE $IFILE
else
  echo -n "" > $IFILE
  for t in "${TESTS[@]}"
  do
      echo -e "$t" >> $IFILE
  done
fi

# Runner
echo -e "$CYAN""minishell test script"$RESET"\nby Araiva"
echo "========================"
$RESTART_CMD
./minishell < $IFILE > $OFILE && cat $OFILE
cat $OFILE | grep -v "$PREFIX_COMMAND" > tmp && cp tmp $OFILE && /bin/rm tmp;

n=$(cat $OFILE | wc -l)
i=1
while [ $i -le $n ]
do
  sed -n "$i,1p" $OFILE
  $((i++))
done

#Cleasing
if [ -f $IFILE ]; then
  /bin/rm $IFILE
fi

# if [ -f $OFILE ]; then
#   /bin/rm $OFILE
# fi

exit 0