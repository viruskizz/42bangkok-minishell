# Constants
RED="\033[0;31m"
GREEN="\033[0;32m"
YELLOW="\033[0;33m"
BLUE="\033[0;34m"
MAGENTA="\033[0;35m"
CYAN="\033[0;36m"
GRAY="\033[0;37m"
LRED="\033[0;91m"
BOLD="\033[1m"
RESET="\033[0m"

# Setup
RESTART_CMD="make restart"
INPUT_FILE=test_input
OUTPUT_FILE=test_output

# Testcase
TEST=(
  "abc"
  '"x"y"z"'
  "'123'"
)

#Generate Test
echo -n "" > $FILE
for t in ${TEST[@]}
do
    echo $t >> $FILE
done

# Runner
echo -e "$CYAN""minishell test script"$RESET"\nby Araiva"
echo "========================"
$RESTART_CMD
# ./minishell < $FILE > $OUTPUT_FILE && cat $OUTPUT_FILE
./minishell < $FILE
