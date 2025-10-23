NAME    = miniRT
CC      = gcc
CFLAGS  = -Wall -Wextra -Werror -g

# Directories
SRC_DIR = src
OBJ_DIR = obj
INC     = -I include -I Libft -I mlx

# Libraries
LIB     = -L ./Libft -lft -L ./mlx -lmlx -lX11 -lXext -lm -lbsd
#LIB     = -L ./Libft -lft
MLX     = mlx/libmlx.a
LFT     = Libft/libft.a
MLXFLAGS = -lm

# Source & Object Files
SRC = $(shell find $(SRC_DIR) -name '*.c')
OBJ = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC))

BLUE    = \033[0;34m
GREEN   = \033[0;32m
RED     = \033[0;31m
YELLOW  = \033[0;33m
BOLD    = \033[1;1m
RESET   = \033[0m

all: $(LFT) mlx $(NAME)

$(NAME): $(OBJ)
	@echo "$(BLUE)$(BOLD)Compiling Mandatory..$(RESET)"
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME) $(LIB) $(MLXFLAGS)
	@echo "$(GREEN) ▄▄▄▄  ▄ ▄▄▄▄  ▄ ▗▄▄▖▗▄▄▄▖  "
	@echo "$(GREEN) █ █ █ ▄ █   █ ▄ ▐▌ ▐▌ █    "
	@echo "$(GREEN) █   █ █ █   █ █ ▐▛▀▚▖ █    "
	@echo "$(GREEN)       █       █ ▐▌ ▐▌ █    "
	@echo "$(GREEN)                            $(RESET)"
	@echo "$(BLUE)$(BOLD)Mandatory Ready!$(RESET)"

# $(NAME): $(OBJ)
# 	@echo "$(BLUE)$(BOLD)Compiling Mandatory..$(RESET)"
# 	$(CC) $(CFLAGS) $(OBJ) -o $(NAME) $(LIB)
# 	@echo "$(GREEN) ▄▄▄▄  ▄ ▄▄▄▄  ▄ ▗▄▄▖▗▄▄▄▖  "
# 	@echo "$(GREEN) █ █ █ ▄ █   █ ▄ ▐▌ ▐▌ █    "
# 	@echo "$(GREEN) █   █ █ █   █ █ ▐▛▀▚▖ █    "
# 	@echo "$(GREEN)       █       █ ▐▌ ▐▌ █    "
# 	@echo "$(GREEN)                            $(RESET)"
# 	@echo "$(BLUE)$(BOLD)Mandatory Ready!$(RESET)"

# Compile .c to .o (with subfolder support)
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

# Libft
$(LFT):
	@echo "$(RED)[ .. ] Compiling Libft..$(RESET)"
	@make -s -C Libft
	@echo "$(GREEN)[ OK ]$(RESET) $(GREEN)Libft ready!$(RESET)"

# MiniLibX
mlx :
	@echo "$(RED)[ .. ] | Compiling minilibx..$(RESET)"
	@make -s -C mlx
	@echo "$(GREEN)[ OK ]$(RESET)|$(GREEN)Minilibx ready!$(RESET)"

clean:
	@rm -rf $(OBJ_DIR)
	@echo "$(RED)$(BOLD)Object files removed.$(RESET)"

fclean: clean
	@make -s fclean -C Libft
	@rm -f $(NAME)
	@echo "$(RED)$(BOLD)All files removed.$(RESET)"

re: fclean all

.SILENT:
.PHONY: all clean fclean re mlx