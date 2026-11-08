#pragma once

#include <string>


enum class Command
{
	INIT,
	DEFAULT
};


struct Commands
{
	private:
		Command cmd;
	public:
		Commands(const std::string& value)
		{
			if (value == "init")
			{
				cmd = Command::INIT;
			}
			else 
			{
				cmd = Command::DEFAULT;

			}

		}


		bool operator==(Command value)
		{
			if (this->cmd == value)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
};
