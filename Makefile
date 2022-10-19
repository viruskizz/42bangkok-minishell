NAME = minishell
CC = gcc
CFLAGS = -Wextra -Wall -Werror

LIBFT_DIR = libft
LREAD_DIR = /opt/homebrew/Cellar/readline/8.1.2

INCLUDE_DIR	= includes
# INCLUDES =	-I$(INCLUDE_DIR) -I$(LIBFT_DIR) -I /opt/homebrew/Cellar/readline/8.1.2/include/
INCLUDES =	-I$(INCLUDE_DIR) \
			-I$(LIBFT_DIR) \
			-I$(LREAD_DIR)/include

LIBS =	-lreadline -L$(LREAD_DIR)/lib  \
		-lft -L$(LIBFT_DIR) \

SRC_DIR = srcs
SRCS = main.c \
	minishell_init.c \
	execution/exec_cmd_table.c \
	execution/exec_signal.c \
	execution/exec_cmd.c \
	execution/extra_cmd.c \
	execution/exec_redirect.c \
	execution/hdoc_convert.c \
	execution/cmd_export.c \
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
	utils/split_cmd.c \
	utils/env_utils.c \
	utils/arr_utils.c \
	utils/str_utils.c \
	utils/ptr_utils.c \
	utils/debug.c

BUILD_DIR = build

OBJS = $(SRCS:%.c=$(BUILD_DIR)/%.o)

all: $(NAME)

$(NAME): $(OBJS) libs
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o $(NAME)

$(OBJS): $(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

norminette:
	@norminette -R CheckForbiddenSourceHeader $(LIBFT_DIR)/*.c
	@norminette -R CheckDefine $(LIBFT_DIR)/libft.h
	@norminette -R CheckForbiddenSourceHeader $(SRC_DIR)/*
	@norminette -R CheckDefine includes/minishell.h includes/color.h

libs:
	@make -C $(LIBFT_DIR)

restart: cbuild $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o $(NAME)

re: fclean all

cbuild:
	$(RM) -rf $(BUILD_DIR)

clean: cbuild
	make clean -C $(LIBFT_DIR)

fclean: cbuild
	@make fclean -C $(LIBFT_DIR)
	$(RM) -f $(NAME)

PHONY: all clean fclean re restart cbuild libs norminette