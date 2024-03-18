#include "RegEx.h"

RegEx::RegEx()
	: m_pattern(std::regex()), m_stringPattern("")
{
	// Empty
}

RegEx::RegEx(const std::string& pattern)
	: m_stringPattern(pattern), m_pattern(std::regex(pattern))
{
	// Empty
}

const std::regex& RegEx::GetRegexPattern() const
{
	// const 1: string returned cannot be modified
	// const 2: qualifier applied to function => function can be called on const objects of RegEx class
	// => calling the function will not modify the object's state
	return m_pattern;
}

const std::string& RegEx::GetStringPattern() const
{
	return m_stringPattern;
}

bool RegEx::CheckWord(const std::string& word)
{
	return std::regex_match(word, m_pattern);
}

bool operator<(const std::pair<char, RegEx>& lhs, const std::pair<char, RegEx>& rhs)
{
	if (lhs.first < rhs.first) {
		return true;
	}
	if (lhs.first == rhs.first) {
		return lhs.second.GetStringPattern() < rhs.second.GetStringPattern();
	}
	return false;
}

std::istream& operator>>(std::istream& is, RegEx& rgx)
{
	return is >> rgx.m_stringPattern;
}

std::ostream& operator<<(std::ostream& os, const RegEx& rgx)
{
	return os << rgx.m_stringPattern;
}
