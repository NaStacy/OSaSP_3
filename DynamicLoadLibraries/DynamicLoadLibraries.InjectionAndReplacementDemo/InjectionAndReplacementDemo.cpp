#include <Windows.h>
#include <string>
#include <iostream>

std::string psReplacement = "this will be partly replaced";

int main()
{
	while (TRUE)
	{
		std::cout << psReplacement << std::endl;
		Sleep(1000);
	}

	return 0;
}
