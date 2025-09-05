# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: yseguin <yseguin@student.42perpignan.fr    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/09/01 15:53:49 by ilbonnev          #+#    #+#              #
#    Updated: 2025/09/05 12:07:24 by yseguin          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ircserv
CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -Iincludes

OBJ_DIR = obj

SRC = src/main.cpp src/Server/Server.cpp
OBJ = $(patsubst %.cpp, $(OBJ_DIR)/%.o, $(SRC))
all: $(NAME)
$(NAME): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all