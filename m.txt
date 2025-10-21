# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: francesca <francesca@student.42.fr>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/04/29 12:20:24 by francesca         #+#    #+#              #
#    Updated: 2025/10/20 16:56:45 by francesca        ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# === Colors ===
GREEN	= \033[0;32m
YELLOW	= \033[0;33m
RED		= \033[0;31m
RESET	= \033[0m
BLUE	= \033[0;34m

# === Project name ===
NAME	= miniRT

# === Compiler settings ===
CC		= cc
CFLAGS = -Wall -Wextra -Werror -g -gdwarf-4 -Iinclude -I$(LIBFT_DIR)
#LDFLAGS	= -L/usr/lib -lX11 -lXext -lm -lbsd

# === Libraries ===
LIBFT_DIR = Libft
LIBFT = $(LIBFT_DIR)/libft.a


# === Directories ===
SRC_DIR = .
OBJ_DIR = obj

# === Sources ===
SRC =	main.c \
		parser/src/utils.c \
		parser/src/check_file.c \
		parser/src/ft_skip_spaces.c \
		parser/src/parse_file.c \
		parser/src/lexer.c \
		parser/src/free.c \
		parser/src/parse_ambient.c \
		parser/src/parse_camera.c \
		parser/src/debug.c \
		parser/src/parse_units.c \
		parser/src/parse_light.c \
		parser/src/lst_function.c \
		parser/src/parse_cylinder.c \
		parser/src/parse_plane.c \
		parser/src/parse_sphere.c \

# === Object files ===
OBJ = $(addprefix $(OBJ_DIR)/, $(SRC:.c=.o))

# === Rules ===
all: $(NAME)

$(NAME): $(LIBFT) $(OBJ)
	@echo "$(GREEN)Linking $(NAME)...$(RESET)"
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJ) $(LIBFT)


$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	@echo "$(BLUE)Compiling $<...$(RESET)"
	@$(CC) $(CFLAGS) -c $< -o $@

$(LIBFT):
	@make -C $(LIBFT_DIR)


clean:
	@echo "$(RED)Cleaning object files...$(RESET)"
	@rm -rf $(OBJ_DIR)
	@make clean -C $(LIBFT_DIR)


fclean: clean
	@echo "$(RED)Removing $(NAME)...$(RESET)"
	@rm -f $(NAME)
	@make fclean -C $(LIBFT_DIR)

re: fclean all

valgrind: $(NAME)
	@valgrind --leak-check=full --show-leak-kinds=all --suppressions=supp.supp ./$(NAME)

NORM_FILES = main.c parser/ env/ execution/ builtins/ header/

norminette:
	norminette $(NORM_FILES)

.PHONY: all clean fclean re norminette