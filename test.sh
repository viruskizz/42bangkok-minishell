# Constants
CYAN="\033[0;36m"
RESET="\033[0m"

# Setup
RESTART_CMD="make restart"
IFILE=itest
OFILE=otest
PREFIX="input command"

# Testcase
## You can add your test here
TESTS=(
  "a\""
  "sr*"
  "\"sr*\""
  "abc"
  "\"xyz\""
  "\"xyz\"'123'"
  "a\"x\"y\"z\"a        ijk"
  "~"
  "'123'"
  "xyz'123'xyz"
  "'x'\$HOME'x'"
  "'x'~'x'"
)

#Generate Test
echo -n "" > $IFILE
for t in "${TESTS[@]}"
do
    echo -e "$t" >> $IFILE
done

# Runner
echo -e "$CYAN""minishell test script"$RESET"\nby Araiva"
echo "========================"
$RESTART_CMD
# ./minishell < $IFILE > $OFILE && cat $OFILE | grep $PREFIX
./minishell < $IFILE > $OFILE && cat $OFILE | grep -v "$PREFIX"

#Cleasing
# if [ -f $IFILE ]; then
#   /bin/rm $IFILE
# fi

# if [ -f $OFILE ]; then
#   /bin/rm $OFILE
# fi

exit 0