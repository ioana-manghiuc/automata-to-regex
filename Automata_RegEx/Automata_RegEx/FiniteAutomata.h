#pragma once
#include "Transition.h"
#include <map>
#include <vector>

class FiniteAutomata
{
public:
	FiniteAutomata();

	int GetStateSize();
	const std::vector<Transition>& GetTransitions() const;
	Transition GetTransitionOf(const std::string& left, const std::string& right) const;

	bool TransitionExists(std::string leftState, std::string symbol, std::string rightState, std::vector<Transition> transitions);
	bool TransitionExists(std::string leftState, std::string rightState, std::vector<Transition> transitions);
	void UniformFiniteAutomata();
	void RemoveTransition(const Transition& t);
	void RemoveState(const std::string& state);

	std::string RandomState(); // choose random non-endpoint state (NOT i or f)
	std::vector<std::pair<std::string, std::string>> GetStatesConnectedVia(const std::string& state) const;
	RegEx GetDirectTransitionLabelBetween(const std::string& lhs, const std::string& rhs);
	RegEx ComputeRegexForStates(const std::string& p, const std::string& k, const std::string& q);
	void ReplaceLabels();

	RegEx RegularExpression();

	friend std::ostream& operator<<(std::ostream& os, const FiniteAutomata& M);

private:
	void ReadTransitionsFromFile();

private:
	std::string m_states;
	std::string m_alphabet;
	char m_initialState;
	std::string m_finalStates;
	std::vector<Transition> m_transitions;
	std::string lambda = "&";
};

