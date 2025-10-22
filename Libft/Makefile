# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: francesca <francesca@student.42.fr>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/04/18 11:58:42 by francesca         #+#    #+#              #
#    Updated: 2025/10/21 11:31:34 by francesca        ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Colors
GREEN	= \033[0;32m
YELLOW	= \033[0;33m
RED	= \033[0;31m
RESET	= \033[0m
BLUE	= \033[0;34m

NAME	= libft.a

CC		= cc
CFLAGS	= -Wall -Wextra -Werror -fPIE

SRC_DIR = .
OBJS_DIR	=	obj/

SRCS = ft_isalpha.c \
       ft_isdigit.c \
       ft_isalnum.c \
       ft_isascii.c \
       ft_isprint.c \
       ft_strlen.c \
       ft_toupper.c \
       ft_tolower.c \
       ft_strchr.c \
       ft_strrchr.c \
       ft_strncmp.c \
       ft_memset.c \
       ft_bzero.c \
       ft_strlcpy.c \
       ft_memchr.c \
       ft_memcmp.c \
       ft_strnstr.c \
       ft_strcmp.c \
       ft_atoi.c \
       ft_calloc.c \
       ft_memcpy.c \
       ft_memmove.c \
       ft_strlcat.c \
       ft_strdup.c \
       ft_substr.c \
       ft_strjoin.c \
       ft_strtrim.c \
       ft_split.c \
       ft_itoa.c \
       ft_putchar_fd.c \
       ft_putstr_fd.c \
       ft_putendl_fd.c \
       ft_putnbr_fd.c \
       ft_strmapi.c \
       ft_striteri.c \
       get_next_line.c \
       ft_lstnew.c \
       ft_lstadd_front.c \
       ft_lstsize.c \
       ft_lstlast.c \
       ft_lstadd_back.c \
       ft_lstdelone.c \
       ft_lstclear.c \
       ft_lstiter.c \
       ft_lstmap.c


OBJS := $(addprefix $(OBJS_DIR), $(SRCS:.c=.o))

# === Custom rule for colorized compilation === #
$(OBJS_DIR)%.o: %.c
	@mkdir -p $(OBJS_DIR)
	
	@$(CC) $(CFLAGS) -c $< -o $@

all: $(NAME) 

$(NAME): $(OBJS)
	@ar rcs $(NAME) $(OBJS)

clean:
	@echo "$(RED)Cleaning Libft object files...$(RESET)"
	@$(RM) -r $(OBJS_DIR)

fclean: clean
	@echo "$(RED)Removing $(NAME)...$(RESET)"
	@$(RM) $(NAME)

re: fclean all

.SILENT:
.PHONY: all bonus clean fclean re
