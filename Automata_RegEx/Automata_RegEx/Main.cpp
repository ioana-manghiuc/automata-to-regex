#include "FiniteAutomata.h"

int main()
{
	FiniteAutomata FA;
	std::cout << FA;
	FA.UniformFiniteAutomata();
	std::cout << std::endl << std::endl;
	std::cout << FA;

	std::string state = FA.RandomState();
	std::cout << state << "\n\n";
	FA.GetStatesConnectedVia("0");
	RegEx rgx = FA.GetDirectTransitionLabelBetween("1", "2");
	std::cout << std::endl << rgx.GetStringPattern() << std::endl;
	std::cout << FA;

	std::cout << std::endl << FA.RegularExpression() << std::endl;
	return 0;
}