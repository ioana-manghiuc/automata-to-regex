#pragma once
#include <iostream>
#include <string>
#include <regex>

class RegEx
{
public:
	RegEx();
	RegEx(const std::string& pattern);

	const std::regex& GetRegexPattern() const;
	const std::string& GetStringPattern() const;
	
	bool CheckWord(const std::string& word);

	friend bool operator<(const std::pair<char, RegEx>& lhs, const std::pair<char, RegEx>& rhs);
	friend std::istream& operator >> (std::istream& is, RegEx& rgx);
	friend std::ostream& operator << (std::ostream& os, const RegEx& rgx);

private:
	std::regex m_pattern{};
	std::string m_stringPattern{};
};

