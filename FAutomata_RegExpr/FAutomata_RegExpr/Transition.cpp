#include "Transition.h"

Transition::Transition()
{
}

Transition::Transition(char leftState, std::string symbol, std::string rightState)
{
	m_left = std::make_pair(leftState, symbol);
	m_right = rightState;
}

Transition::Transition(std::pair<char, std::string> left, std::string right) :
	m_left(left), m_right(right)
{
}

std::pair<char, std::string> Transition::GetArguments() const
{
	return m_left;
}

std::string Transition::GetResult() const
{
	return m_right;
}

bool Transition::IsSelfTransition()
{
	return m_left.first == m_right[0];
}

std::istream& operator>>(std::istream& is, Transition& t)
{
	char left_one, left_two;
	is >> left_one >> left_two >> t.m_right;
	t.m_left = std::make_pair(left_one, left_two);
	return is;
}

std::ostream& operator<<(std::ostream& os, const Transition& t)
{
	return os << "transition(" << t.m_left.first << ", " << t.m_left.second << ") = " << t.m_right << "\n";
}