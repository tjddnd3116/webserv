# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: soum <soum@student.42seoul.kr>             +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/04/14 14:41:39 by soum              #+#    #+#              #
#    Updated: 2022/10/11 14:36:37 by soum             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = webserv
CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -g -fsanitize=address

SRCS = ./src/main.cpp \
	   ./src/WsInitializer.cpp \
	   ./src/WsException.cpp \
	   ./src/WsServer.cpp \
	   ./src/parser/configParser.cpp \
	   ./src/parser/fileReader.cpp \
	   ./src/parser/tokenizer.cpp \
	   ./src/parser/WsConfigInfo.cpp \
	   ./src/method/WsIMethod.cpp \
	   ./src/method/WsPutMethod.cpp \
	   ./src/method/WsPostMethod.cpp \
	   ./src/method/WsGetMethod.cpp \
	   ./src/socket/WsASocket.cpp \
	   ./src/socket/WsServerSock.cpp \
	   ./src/socket/WsClientSock.cpp \
	   ./src/socket/WsRequest.cpp \
	   ./src/socket/WsResponse.cpp \

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
