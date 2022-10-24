#!/bin/bash
##################################
#     Minishell Testscript       #
#   created by Araiva (tsomsa)   #
##################################

# Your Setup

# your exec file
EXEC="minishell"

# restart command in Makefile
RESTART_CMD="make restart"

# Prefix shell command
PREFIX_COMMAND="input command"

# Testcase file, use -f as flag to apply specfic test for `test_mnshell.sh`
TEST_FILE="testcase"

# Enalbling interative mode without generated tracing test result, use -i flag for `test_mnshell.sh`
INTERACT=0

# Enabling Valgrind to check leaks, use -l or -L as flag for `test_mnshell.sh`
VALGRIND=""

# Enabling Cleasing test directory, use -l or -L as flag for `test_mnshell.sh`


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
IFILE=test/input.txt
RFILE=test/result.txt
OFILE=test/output.txt
EFILE=test/expect.txt
DFILE=test/diff.txt
XFILE=test/error.txt
ZFILE=test/xrror.txt
TURL="https://raw.githubusercontent.com/viruskizz/42bangkok_minishell/develop/test.txt"

# Get option from command line
usage() {
  echo "Usage: $0 [ -f file ] [-i interative mode] [-l leak check] [-L leak check full] [-h help]"
}
while getopts "f:ilLch" options; do
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
  printf "${YELLOW}%60s\n${RESET}" "by Araiva"
  echo "============================================================"
  generate
  runner
  printoutput
  echo -e $YELLOW"== FINISH =="$RESET
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
    exit 0
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
  echo -e $MAGENTA"RESULT"$RESET
  echo -e $BOLD"Correct:$RESET $correct/$n"
}

banner() {
# credit image to ascii from `https://ascii-generator.site`
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
}

## Run main()

main;
exit 0