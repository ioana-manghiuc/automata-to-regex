#include "Transition.h"

Transition::Transition()
{
	// Empty
}

Transition::Transition(std::string leftState, RegEx rgx, std::string rightState)
	: m_left{ std::make_pair(leftState, rgx) }, m_right{ rightState }
{
	// Empty
}

Transition::Transition(std::pair<std::string, RegEx> left, std::string right) :
	m_left{ left }, m_right{ right }
{
	// Empty
}

std::pair<std::string, RegEx> Transition::GetArguments() const
{
	return m_left;
}

std::string Transition::GetResult() const
{
	return m_right;
}

void Transition::SetRegexLabel(const RegEx& rgx)
{
	m_left.second = rgx;
}

bool Transition::IsSelfTransition()
{
	return m_left.first == m_right;
}

bool Transition::operator==(const Transition& other) const
{
	return m_left.first == other.m_left.first
		&& m_left.second == other.m_left.second
		&& m_right == other.m_right;
}

std::istream& operator>>(std::istream& is, Transition& t)
{
	std::string left_one, left_two;
	is >> left_one >> left_two >> t.m_right;
	RegEx rgx(left_two);
	t.m_left = std::make_pair(left_one, rgx);
	return is;
}

std::ostream& operator<<(std::ostream& os, const Transition& t)
{
	return os << "transition(" << t.m_left.first << ", " << t.m_left.second << ") = " << t.m_right << "\n";
}
