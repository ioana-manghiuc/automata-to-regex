#pragma once
#include <iostream>
#include "RegEx.h"

class Transition
{
public:
	Transition();
	Transition(char leftState, RegEx rgx, std::string rightState);
	Transition(std::pair<char, RegEx>left, std::string right);

	std::pair<char, RegEx> GetArguments() const;
	std::string GetResult() const;

	bool IsSelfTransition();

	friend std::istream& operator>>(std::istream& is, Transition& t);
	friend std::ostream& operator<<(std::ostream& os, const Transition& t);

private:
	std::pair<char, RegEx>m_left;
	std::string m_right;
};

