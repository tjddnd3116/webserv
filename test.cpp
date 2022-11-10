#include <iostream>
#include <fstream>

int main(void)
{
	std::ofstream createFile("/Users/gson/webserv/aaa");
	if (createFile.fail())
		std::cout << "file open fail" << std::endl;
	createFile << "hello 42";
	createFile.close();

	return (0);
}

