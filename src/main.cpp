#include <exception>
#include <iostream>

#include "WsException.hpp"
#include "./parser/parser.hpp"
#include "server.hpp"

int main(int argc, char **argv)
{
	try
	{
		if (argc != 2)
			throw (WsException("invalid arguments count"));

		parser parser(argv[1]);
		parser.configParse();

		server	server(parser.getConfigInfo());
		server.createServerSock();
		server.run();
	}
	catch (std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		// TODO
		// free all data
		// clear();
		std::cerr << "Webserv server exit....." << std::endl;
	}
	return (0);
}
