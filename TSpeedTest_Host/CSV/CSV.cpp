#include "CSV.h"

std::vector<std::string> CSV::Parse(const char* _string)
{
	std::vector<std::string> result;

	char* token = nullptr;
	char* progress = nullptr;
	char* string = const_cast<char*>(_string);

	result.push_back(strtok_s(string, ",", &progress));

	while (true)
	{
		token = strtok_s(nullptr, ",", &progress);

		if (token == nullptr || *token == '\0' || *token == '\n')   break;
		result.push_back(token);
	}

	return result;
}
