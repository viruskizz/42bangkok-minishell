NAME = minishell
CC = gcc
# CFLAGS = -Wextra -Wall -Werror

INCLUDE_DIR	= includes
INCLUDES =	-I$(INCLUDE_DIR)

LIBS = -lreadline

SRC_DIR = srcs
SRCS = minishell.c \
	minishell_split.c \
	minishell_util1.c

BUILD_DIR = build

OBJS = $(SRCS:%.c=$(BUILD_DIR)/%.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o $(NAME)

$(OBJS): $(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	$(RM) -rf $(BUILD_DIR)

fclean: clean
	$(RM) -f $(NAME)

re: fclean all

PHONY: all clean fclean re