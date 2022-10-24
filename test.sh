#!/bin/bash
##################################
#     Minishell Testscript       #
#   created by Araiva (tsomsa)   #
##################################

### Your Setup
##
# your exec file
EXEC="minishell"

# restart command in Makefile
RESTART_CMD="make re"

# Prefix shell command
PREFIX_COMMAND="input command"

# Testcase file, use -f as flag to apply specfic test for `test_mnshell.sh`
TEST_FILE="testcase"


##################################
#### DO NOT change code below ####
###
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
UL="\033[4m"
VALGRIND=""
INTERACT=0
BANNER=1
SLP=15
IFILE=test/input.txt
RFILE=test/result.txt
OFILE=test/output.txt
EFILE=test/expect.txt
DFILE=test/diff.txt
XFILE=test/error.txt
ZFILE=test/xrror.txt
TURL="https://gist.githubusercontent.com/viruskizz/c53fefe8f0ef08cc56e97f56ae6ce6c1/raw/47c0e230a690c473acef9539f8dee29717455480/minishell_testcase.sh"

# Get option from command line
usage() {
  echo "Usage: $0 [ -f file ] [-i interative mode] [-l leak check] [-L leak check full] [-n no banner] [-h help]"
}
while getopts "f:ilLnh" options; do
  case "$options" in
    f)
      TEST_FILE=$OPTARG
      ;;
    i)
      INTERACT=1
      ;;
    l)
      VALGRIND="valgrind"
      ;;
    L)
      VALGRIND="valgrind --leak-check=full"
      ;;
    n)
      BANNER=0
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
  $RESTART_CMD
  echo -e $MAGENTA"Minishell test script"$RESET
  banner
  echo "========================= START ============================"
  generate
  delay
  runner
  printoutput
  echo "======================== FINISH ============================"
}

generate() {
  if [ ! -d "test/" ]; then
    mkdir test/
  fi

  if [ ! -f $TEST_FILE ]; then
    echo -e $RED"NOT_FOUND: $TEST_FILE"$RESET
    remote=$(wget --spider -q $TURL)
    if [ $? -eq 0 ]
    then
      echo "Download test file from internet"
      wget -O $TEST_FILE $TURL -q --show-progress
    else
      echo "create example $TEST_FILE"
      echo "echo example" > $TEST_FILE
      exit 1
    fi
  fi
  if [ -f $TEST_FILE ]; then
    echo -e $CYAN"File:"$RESET $TEST_FILE
    cat $TEST_FILE | grep -v "^#" > $IFILE
  fi
}

runner() {
  # Runner
  if [ $INTERACT -eq 1 ]; then
    echo "START execute test..."
    echo "------------------------------------------------------------"
    $VALGRIND ./$EXEC < $IFILE
    if [ -d "test/" ]; then
      rm -rf test/
    fi
  else
    $VALGRIND ./$EXEC < $IFILE > $RFILE 2> $XFILE
    while read -r line; do eval "$line"; done < $IFILE > $EFILE 2> $ZFILE
    cat $RFILE | grep -v "$PREFIX_COMMAND" > $OFILE
    diff $OFILE $EFILE > $DFILE
  fi
}

printoutput() {
  if [ $INTERACT -eq 1 ]; then
    return 0
  fi
  n=$(cat $OFILE | wc -l)
  i=1
  correct=0
  wrong=0
  printf "${BOLD}NO. | Input %48s\n${RESET}"  "Mark" 
  echo "------------------------------------------------------------"
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
    printf "%02d: %-55s" $i "$input"
    if [[ $check == true ]]; then
      echo -e $GREEN"✓"$RESET
    else
      echo -e $RED"X"$RESET
      echo -e "$CYAN""output:" $output $RESET
      echo -e "$GREEN""expect:" $expect $RESET
    fi 
    i=$(( i + 1 ))
  done
  echo "------------------------------------------------------------"
  echo -e $GRAY": Tracing your test result in test/ directory :"$RESET
  echo -e $MAGENTA$UL$BOLD"RESULT"$RESET
  echo -e $YELLOW$BOLD"Correct:$RESET $correct/$n"
}

banner() {
# credit image to ascii from `https://ascii-generator.site`
if [ $BANNER -eq 0 ]; then
  printf "${YELLOW}%s\n${RESET}" "by Araiva"
  return 0
fi
cat << EOF
                                                            
                           .                                
                        .**+**+=:.                          
                        *#********+=:..                     
                 :+**=:+###***++********+-.                 
                =***###%#%##****++*********+:               
              -****###%%%%###******+*********+.             
            .+#***###%%%%%%#%%#****************=.           
          .++#***%#*#%%%%%#######****************:          
         :: -***#%**%---:..=#=*#####**************-         
            +**#%#*#*:...:::#=.***##:=*##******###*:        
           :***###*#-.....::+-..***#-..*%###****#%#*.       
           -**####*#:......:+:...***+..:+###%#######        
           =**####**::----...-:..:**#++=-+#*########-       
           +*#####*++#%%%=.....::.:**%@@@#=*+###*-#%*=:     
           +*####%#+-%@@@:.......:.:**@@@*.+-###*--#+       
           +#%##*##=:@%@%:..........:#%#%*.:-*##+-: =       
           +**####*-:#*##............-#*#=:--##*=-          
           +. :###*-:-%#=.............-*+::--*#**           
               :#**--:::.....____......:::--==*:.           
                *#=---:::::::::::::::::---=+: .             
                :#=  -++==---=-=---:::-*+==                 
                .:.      -:===-*+---=+..                    
                         .#%=:.###%%%%+                     
                        -%%#:. *###%%##+                    
                   .:. *%#*+.. =*****%###.                  
                 :+==+%%%#*+======+*#*%##*                  
            .:-==+=---:. -%%*##*#%#%%.-**=                  
          .:::----:      .%#%%*-+%%%%                       
       ...:::..           +#%%-  *%%#                       
      ....                 %%%.  -%%#                       
                           -+=   .+*+                       
                                                            
EOF
printf "${YELLOW}%60s\n${RESET}" "by Araiva"
}

delay() {
  if [ $BANNER -eq 0 ]; then
    return 0
  fi
  echo -n "Testing"
  for i in $(seq 1 $SLP); do
    echo -n "."
    sleep 0.1
  done
  echo ""
}
## Run main()

main;
exit 0