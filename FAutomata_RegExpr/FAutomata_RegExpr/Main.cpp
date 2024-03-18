#include "FiniteAutomata.h"

int main()
{
	FiniteAutomata FA;
	std::cout << FA;
	FA.UniformFiniteAutomata();
	std::cout << std::endl << std::endl;
	std::cout << FA;
	std::cout << FA.RandomState();
	return 0;
}