# Makefile for minishell
# .SILENT:

# Variables
NAME=minishell

# Prints DEBUG Messages
DEBUG = 0

# Compiler options
CC = cc
CFLAGS = -D DEBUG=$(DEBUG) -Wall -Werror -Wextra #-g -fsanitize=address -fsanitize-address-use-after-scope
CLIBS = -L$(LIB_FOLDER) -lft -lm -lreadline
CINCLUDES  = -I$(INCLUDE_FOLDER) 
RM = rm -rf

# ->Colors
RED    = 	\033[0;31m
GREEN  = 	\033[0;32m
ORANGE = 	\033[0;33m
BLUE   = 	\033[34m
PURPLE = 	\033[35m
RESET  = 	\033[0m

# ->Folders
INCLUDE_FOLDER = ./includes/
SRC_FOLDER     = ./src/
OBJ_FOLDER     = ./obj/
LIB_FOLDER     = ./libft/

# ->Files
LIBFT = $(LIB_FOLDER)libft.a
SRCS = $(addprefix $(SRC_FOLDER), 				\
	0-Core/main.c								\
	0-Core/env.c								\
	0-Core/general_utils_1.c					\
	0-Core/input_manager.c						\
	0-Core/manage_minibox.c						\
	0-Core/signals.c							\
	1-Input-Management/lexer/expand_vars.c		\
	1-Input-Management/lexer/lexer.c 			\
	1-Input-Management/lexer/quotes_handler.c	\
	1-Input-Management/lexer/lexer_utils.c		\
	1-Input-Management/parser/parser.c			\
	1-Input-Management/parser/job.c				\
	1-Input-Management/parser/command.c			\
	1-Input-Management/parser/token_list.c		\
	1-Input-Management/parser/redir.c			\
	1-Input-Management/parser/redir_in.c		\
	1-Input-Management/parser/redir_out.c		\
	1-Input-Management/parser/parser_utils.c	\
	2-Execution/builtins/cd.c					\
	2-Execution/builtins/echo.c					\
	2-Execution/builtins/pwd.c					\
	2-Execution/builtins/export.c				\
	2-Execution/builtins/unset.c				\
	2-Execution/builtins/env.c					\
	2-Execution/builtins/exit.c					\
	)

# Object files
OBJS = $(SRCS:$(SRC_FOLDER)%.c=$(OBJ_FOLDER)%.o)

#++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++#

# TARGETS
.PHONY: all clean fclean re norm

all: $(NAME)

$(NAME): $(LIBFT) $(OBJS) 
	@$(CC) $(OBJS) $(CFLAGS) $(CLIBS) $(CINCLUDES) -o $(NAME)
	@echo "\n$(GREEN)$(NAME): created$(RESET)"

$(OBJ_FOLDER)%.o: $(SRC_FOLDER)%.c
	@mkdir -p $(@D)
	@echo -n "$(BLUE).$(RESET)"
	@$(CC) $(CFLAGS) $(CINCLUDES)-c $< -o $@

$(LIBFT):
	@echo "$(ORANGE)compiling: $(LIBFT)\n$(RESET)"
	@$(MAKE) -sC $(LIB_FOLDER) DEBUG=$(DEBUG)

clean:
	@$(RM) $(OBJ_FOLDER) readline.supp
	@echo "$(BLUE)┌────────────────────┐"
	@echo "│    $(ORANGE)[✓] CLEANED!$(BLUE)    │"
	@echo "$(BLUE)└────────────────────┘$(RESET)"

fclean: clean
	@make -sC $(LIB_FOLDER) fclean
	@$(RM) $(NAME)

re: fclean all

test: all
	gnome-terminal --window --hide-menubar --title "ASTEINS MINITALK TESTER" --geometry=60x10+630+100 --working-directory="$(CURDIR)$(TEST_FOLDER)" -- "./run.sh" &

norm:
	norminette

readline.supp:
	wget https://raw.githubusercontent.com/benjaminbrassart/minishell/master/readline.supp
valgrind:$(NAME) readline.supp
	valgrind --leak-check=full --show-leak-kinds=all --suppressions=readline.supp ./$(NAME)