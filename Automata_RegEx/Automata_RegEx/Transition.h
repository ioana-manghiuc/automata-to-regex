#pragma once
#include <iostream>
#include "RegEx.h"

class Transition
{
public:

	Transition();
	Transition(std::string leftState, RegEx rgx, std::string rightState);
	Transition(std::pair<std::string, RegEx>left, std::string right);

	std::pair<std::string, RegEx> GetArguments() const;
	std::string GetResult() const;
	void SetRegexLabel(const RegEx& rgx);

	bool IsSelfTransition();

	bool operator==(const Transition& other) const;
	friend std::istream& operator>>(std::istream& is, Transition& t);
	friend std::ostream& operator<<(std::ostream& os, const Transition& t);

private:
	std::pair<std::string, RegEx>m_left;
	std::string m_right;
};

