# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hsabir <marvin@42.fr>                      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/12/14 13:27:39 by hsabir            #+#    #+#              #
#    Updated: 2022/03/01 15:46:14 by lgyger           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #
CC = c++

CFLAGS = -Wall -Wextra -Werror -std=c++98 -o
NAME = ex00
FILES = main.cpp\
	Socket.cpp\
	BindingSocket.cpp

all : $(NAME)
OBJ = $(FILES:%.cpp=%.o)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(NAME) $(OBJ)
fclean: clean
	rm -rf $(NAME)
clean:	
	rm -f $(OBJ)
re: fclean clean all

.PHONY: clean fclean all re
