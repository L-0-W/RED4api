#include <iostream>
#include "Commands/Commands.hpp"

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		std::cout << "Sem argumentos" << std::endl;
		return 0;
	}


	Commands cmd = std::string(argv[1]);

	if (cmd == Command::DEFAULT)
	{
		std::cout << "Executado: {DEFAULT}" << std::endl;
	}

	if (cmd == Command::INIT)
	{
		std::cout << "Executado: {INIT}" << std::endl;
	}

	return 0;
}