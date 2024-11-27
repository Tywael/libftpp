# Project: libftpp

## Variables
NAME = libftpp.a
TESTS = main_pool

SRCS = 
OBJS = $(SRCS:.c=.o)

SRCS_DIR = src
OBJS_DIR = obj
TEST_DIR = test/fromSubject

HEADERS = -I inc

## Paths
_SRCS = $(addprefix $(SRCS_DIR)/, $(SRCS))
_OBJS = $(addprefix $(OBJS_DIR)/, $(OBJS))

## Commands
CC = g++
CFLAGS = -Wall -Wextra -Werror -std=c++14
CFLAGS += $(HEADERS)
MAKE = make --no-print-directory

## Colors
_CLEAR		= \033[0K\r\c
_OK			= [\033[32mOK\033[0m]
_RM			= [\033[31mRM\033[0m]

## Rules
all: $(NAME) $(TESTS)

$(NAME): $(_OBJS)
	@ar rc $(NAME) $(_OBJS)
	@echo "$(_OK) $(NAME) compiled"

$(_OBJS_DIR)/%.o: $(_SRCS_DIR)/%.cpp
	@echo "Compiling $< ... $*.c\r\c"
	@mkdir -p $(OBJS_DIR)
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "$(_CLEAR)"

$(TESTS): %: $(TEST_DIR)/%.o $(NAME)
	@$(CC) $(CFLAGS) -o $@ $^
	@echo "$(_OK) $@ compiled"

$(TEST_DIR)/%.o: $(TEST_DIR)/%.cpp
	@echo "Compiling $< ... $*.cpp\r\c"
	@mkdir -p $(OBJS_DIR)
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "$(_OK) $< compiled"

clean:
	@rm -rf $(OBJS_DIR)

fclean: clean
	@rm -f $(NAME)
	@echo "$(_RM) $(NAME) \t\tfull clean"

re: fclean all

.PHONY: all clean fclean re

