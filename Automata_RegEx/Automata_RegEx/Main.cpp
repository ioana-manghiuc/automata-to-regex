#include "FiniteAutomata.h"

int main()
{
	FiniteAutomata FA;
	std::cout << FA;

	RegEx rgx = FA.RegularExpression();
	std::cout << "\nREGULAR EXPRESSION: " << rgx << std::endl;

    while (true)
    {
        std::string input;
        std::cout << "\nWORD: ";
        std::getline(std::cin, input);

        if (input.empty()) {
            break; 
        }

        if (rgx.CheckWord(input))
        {
            std::cout << " --> word accepted\n";
        }
        else
        {
            std::cout << " --> word NOT accepted\n";
        }
    }

	return 0;
}