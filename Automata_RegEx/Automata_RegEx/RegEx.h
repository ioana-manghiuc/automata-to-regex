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

	RegEx Union(const RegEx& rgx) const;
	RegEx Concatenation(const RegEx& rgx) const;
	RegEx KleeneStar() const;

	void RemoveLambdas();

	bool CheckWord(const std::string& word);

	bool operator==(const RegEx& rgx) const;
	friend bool operator<(const std::pair<std::string, RegEx>& lhs, const std::pair<std::string, RegEx>& rhs);
	friend std::istream& operator >> (std::istream& is, RegEx& rgx);
	friend std::ostream& operator << (std::ostream& os, const RegEx& rgx);

private:
	std::regex m_pattern;
	std::string m_stringPattern;
};

