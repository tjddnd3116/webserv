# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: gson <gson@student.42.fr>                  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/04/14 14:41:39 by soum              #+#    #+#              #
#    Updated: 2022/10/30 18:38:53 by gson             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = webserv
CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -g -fsanitize=address

SRCS = ./src/main.cpp \
	   ./src/webServ.cpp \
	   ./src/WsException.cpp \
	   ./src/server.cpp \
	   ./src/parser/parser.cpp \
	   ./src/parser/fileReader.cpp \
	   ./src/parser/tokenizer.cpp \
	   ./src/parser/configInfo.cpp \
	   ./src/method/AMethod.cpp \
	   ./src/method/putMethod.cpp \
	   ./src/method/postMethod.cpp \
	   ./src/method/headMethod.cpp \
	   ./src/method/getMethod.cpp \
	   ./src/method/deleteMethod.cpp \
	   ./src/socket/ASocket.cpp \
	   ./src/socket/serverSocket.cpp \
	   ./src/socket/clientSocket.cpp \
	   ./src/socket/request.cpp \
	   ./src/socket/response.cpp \
	   ./src/cgi/cgi.cpp \

.cpp.o :
	$(CXX) $(CXXFLAGS) -c $< -o $(<:.cpp=.o)

OBJS = $(SRCS:.cpp=.o)

all : $(NAME)

$(NAME) : $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

debug : all
	$(CXX) $(CXXFLAGS) -g $(SRCS) -o $(NAME)
	$(shell cp ~/vimspector.json ./.vimspector.json)
	$(shell perl -pi -e 's/filename/webserv/g' .vimspector.json)

clean :
	rm -f $(OBJS)
	rm -f .vimspector.json
	rm -rf $(NAME).dSYM

fclean : clean
	rm -f $(NAME)

re : clean all

.PHONY: all clean fclean re debug
