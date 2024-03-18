#include "FiniteAutomata.h"

int main()
{
	FiniteAutomata FA;
	std::cout << FA;
	FA.UniformFiniteAutomata();
	std::cout << std::endl << std::endl;
	std::cout << FA;

	RegEx rgx = FA.RegularExpression();
	rgx.SetRegexPattern(rgx.GetStringPattern());
	std::cout << std::endl << rgx << std::endl;
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