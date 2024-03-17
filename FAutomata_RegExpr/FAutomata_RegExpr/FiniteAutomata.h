#pragma once
#include "Transition.h"
#include <vector>

class FiniteAutomata
{
public:
	FiniteAutomata();

	int GetStateSize();
	const std::vector<Transition>& GetTransitions();

	bool TransitionExists(char leftState, std::string symbol, std::string rightState, std::vector<Transition> transitions);
	void UniformFiniteAutomaton();

	char RandomState(); // choose random non-endpoint state (NOT i or f)

	// compute regular expression
	// change transition label
	// simplify regex path

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