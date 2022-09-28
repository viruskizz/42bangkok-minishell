NAME = minishell
CC = gcc
# CFLAGS = -Wextra -Wall -Werror

LIBFT_DIR = libft

INCLUDE_DIR	= includes
INCLUDES =	-I$(INCLUDE_DIR) -I$(LIBFT_DIR)

LIBS = -lreadline \
	-L$(LIBFT_DIR) -lft

SRC_DIR = srcs
SRCS = main.c \
	minishell.c \
	minishell_redirect.c \
	minishell_split.c \
	minishell_util1.c \
	exp_env.c \
	split_input.c \
	validate_token.c \
	parse_token.c \
	group_cmd.c \
	utils/quoting.c \
	utils/metachar.c \
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