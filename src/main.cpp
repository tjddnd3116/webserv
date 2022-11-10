#include <exception>
#include <iostream>

#include "./parser/parser.hpp"
#include "WsException.hpp"
#include "server.hpp"
#include "webServ.hpp"

int main(int argc, char **argv)
{
	try
	{
		if (argc != 2)
			throw (WsException("invalid arguments count"));
		webServ webServ;
		webServ.parsing(argv[1]);
		webServ.serverRun();
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		// TODO
		// free all data
		// webServ.clear();
		std::cerr << "Webserv server exit....." << std::endl;
	}
	return (0);
}
