#pragma once
#include <iostream>
#include <string>

class Transition
{
public:
	Transition();
	Transition(char leftState, std::string symbol, std::string rightState);
	Transition(std::pair<char, std::string>left, std::string right);

	std::pair<char, std::string> GetArguments() const;
	std::string GetResult() const;

	bool IsSelfTransition();

	friend std::istream& operator>>(std::istream& is, Transition& t);
	friend std::ostream& operator<<(std::ostream& os, const Transition& t);

private:
	std::pair<char, std::string>m_left;
	std::string m_right;
};

