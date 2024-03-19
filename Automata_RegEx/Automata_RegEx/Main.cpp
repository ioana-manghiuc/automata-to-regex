#include "FiniteAutomata.h"

int main()
{
	FiniteAutomata FA;
	std::cout << FA;

	RegEx rgx = FA.RegularExpression();
	std::cout << "\nREGULAR EXPRESSION: " << rgx << std::endl;

	if (rgx.CheckWord("abceeedab"))
	{
		std::cout << "\nword accepted\n";
	}
	else
	{
		std::cout << "\nword not accepted\n";
	}

	return 0;
}