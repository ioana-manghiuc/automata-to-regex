#pragma once
#include "Transition.h"
#include <map>
#include <vector>

class FiniteAutomata
{
public:
	FiniteAutomata();

	const std::vector<Transition>& GetTransitions() const;

	bool TransitionExists(std::string leftState, std::string symbol, std::string rightState, std::vector<Transition> transitions);
	void RemoveTransition(const Transition& t);
	void RemoveState(const std::string& state);

	std::string RandomState(); 
	std::multimap<std::string, std::string> GetStatesConnectedVia(const std::string& state) const;
	RegEx GetDirectTransitionLabelBetween(const std::string& lhs, const std::string& rhs);
	RegEx ComputeRegexForStates(const std::string& p, const std::string& k, const std::string& q);
	void ReplaceLabels();

	RegEx RegularExpression();

	friend std::ostream& operator<<(std::ostream& os, const FiniteAutomata& M);

private:
	void UniformFiniteAutomata();
	void ReadTransitionsFromFile();

private:
	std::vector<std::string> m_states;
	std::string m_alphabet;
	std::string m_initialState;
	std::vector<std::string> m_finalStates;
	std::vector<Transition> m_transitions;
	std::string lambda = "&";
};

