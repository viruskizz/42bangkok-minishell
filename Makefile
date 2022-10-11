NAME = minishell
CC = gcc
# CFLAGS = -Wextra -Wall -Werror

LIBFT_DIR = libft

INCLUDE_DIR	= includes
INCLUDES =	-I$(INCLUDE_DIR) -I$(LIBFT_DIR) -I /opt/homebrew/Cellar/readline/8.1.2/include/
# INCLUDES =	-I$(INCLUDE_DIR) -I$(LIBFT_DIR)

LIBS = -lreadline \
	-L$(LIBFT_DIR) -lft \
	-L /opt/homebrew/Cellar/readline/8.1.2/lib
# LIBS = -lreadline \
# 	-L$(LIBFT_DIR) -lft

SRC_DIR = srcs
SRCS = main.c \
	minishell_redirect.c \
	minishell_environment.c \
	minishell_execution.c \
	minishell_execution_command.c \
	minishell_commands.c \
	minishell_export.c \
	minishell_unset.c \
	minishell_split.c \
	minishell_util1.c \
	minishell_util2.c \
	input_handling/split_input.c \
	input_handling/validate_token.c \
	input_handling/parse_token.c \
	input_handling/group_cmd.c \
	utils/exp_env.c \
	utils/exp_str.c \
	utils/wild_paths.c \
	utils/quoting.c \
	utils/metachar.c \
	utils/parameter.c \
	utils/arr_utils.c \
	utils/debug.c

BUILD_DIR = build

OBJS = $(SRCS:%.c=$(BUILD_DIR)/%.o)

all: $(NAME)

$(NAME): $(OBJS) libs
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o $(NAME)

$(OBJS): $(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

libs:
	@make -C $(LIBFT_DIR)

restart: cbuild $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o $(NAME)

re: fclean all

cbuild:
	$(RM) -rf $(BUILD_DIR)

clean: cbuild
	make clean -C $(LIBFT_DIR)

fclean: clean
	make fclean -C $(LIBFT_DIR)
	$(RM) -f $(NAME)



PHONY: all clean fclean re