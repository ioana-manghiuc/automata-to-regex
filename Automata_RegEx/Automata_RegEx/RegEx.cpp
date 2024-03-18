#include "RegEx.h"

RegEx::RegEx()
	: m_pattern{ std::regex() }, m_stringPattern{ "" }
{
	// Empty
}

RegEx::RegEx(const std::string& pattern)
	: m_stringPattern{ pattern }, m_pattern{ std::regex(pattern) }
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

RegEx RegEx::Union(const RegEx& rgx) const
{
	if (m_stringPattern.empty()) { return rgx; }
	if (rgx.m_stringPattern.empty()) { return *this; }
	return { m_stringPattern + "+" + rgx.m_stringPattern };
}

RegEx RegEx::Concatenation(const RegEx& rgx) const
{
	return { m_stringPattern + rgx.m_stringPattern };
}

RegEx RegEx::KleeneStar() const
{
	if (m_stringPattern.empty()) { return m_stringPattern; }
	if (m_stringPattern.size() == 1) { return { m_stringPattern + "*" }; }
	if (m_stringPattern[0] == '(' && m_stringPattern[m_stringPattern.size() - 1] == ')')
	{
		return { m_stringPattern + "*" };
	}
	return { "(" + m_stringPattern + ")*" };
}

void RegEx::RemoveLambdas()
{
	m_stringPattern.erase(std::remove_if(m_stringPattern.begin(), m_stringPattern.end(),
		[](char c) { return c == '&'; }),
		m_stringPattern.end());
}

bool RegEx::CheckWord(const std::string& word)
{
	return std::regex_match(word, m_pattern);
}

bool RegEx::operator==(const RegEx& rgx) const
{
	return m_stringPattern == rgx.m_stringPattern;
}

bool operator<(const std::pair<std::string, RegEx>& lhs, const std::pair<std::string, RegEx>& rhs)
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
