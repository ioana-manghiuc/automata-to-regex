#include "FiniteAutomata.h"
#include <map>
#include <fstream>
#include <random>

FiniteAutomata::FiniteAutomata()
{
	ReadTransitionsFromFile();
}

const std::vector<Transition>& FiniteAutomata::GetTransitions()
{
	return m_transitions;
}

int FiniteAutomata::GetStateSize()
{
	return std::count_if(m_states.begin(), m_states.end(), [](char c) {
		return c != ' ';
		});
}

bool FiniteAutomata::TransitionExists(char leftState, std::string symbol, std::string rightState, std::vector<Transition> transitions)
{
	for (Transition t : transitions)
	{
		if (t.GetArguments().first == leftState && t.GetArguments().second.GetStringPattern() == symbol
			&& t.GetResult() == rightState)
		{
			return true;
		}
	}
	return false;
}

char FiniteAutomata::RandomState()
{
	std::random_device RD; // random device to generate a seed for the random number engine
	std::mt19937 engine(RD()); // Mersenne Twister pseudo-random number engine, seeded with the random device
	std::uniform_int_distribution<> distr(0, m_states.size() - 1); // uniform distribution for integers within the size

	char randState;
	do
	{
		randState = m_states[distr(engine)]; 
	} while (randState == m_initialState || randState == ' ' ||
			m_finalStates.find(randState) != std::string::npos);
	
	return randState;
}

void FiniteAutomata::UniformFiniteAutomata()
{
	char newInitialState = 'i';
	char newFinalState = 'f';
	std::string initState(1, m_initialState);
	std::string finalState(1, newFinalState);
	RegEx rgx(lambda);

	std::vector<Transition> newTransitions = m_transitions;

	if (!TransitionExists(newInitialState, lambda, initState, newTransitions))
	{
		Transition newTransition(newInitialState, rgx, initState);
		newTransitions.push_back(newTransition);
	}

	for (char c : m_finalStates)
	{
		if (c != ' ' && !TransitionExists(c, lambda, finalState, newTransitions))
		{
			Transition newTransition(c, rgx, finalState);
			newTransitions.push_back(newTransition);
		}
	}

	if ((m_states.find(newInitialState) == std::string::npos) &&
		(m_states.find(newFinalState) == std::string::npos))
	{
		m_states.push_back(' ');
		m_states.push_back(newInitialState);
		m_states.push_back(' ');
		m_states.push_back(newFinalState);
	}

	m_transitions = newTransitions;
	m_initialState = newInitialState;
	m_finalStates = finalState;	
}


void FiniteAutomata::ReadTransitionsFromFile()
{
	std::ifstream input("automata.txt");

	if (input.is_open())
	{
		std::getline(input, m_states);
		std::getline(input, m_alphabet);		
		std::getline(input, m_finalStates);
		input >> m_initialState;
		int size;
		input >> size;
		Transition transition;
		for (int i = 0; i < size; i++)
		{
			input >> transition;
			m_transitions.push_back(transition);
		}
	}
	else
	{
		std::cerr << "error opening file.\n";
	}
	input.close();
}



std::ostream& operator<<(std::ostream& os, const FiniteAutomata& M)
{
	os << "M = ({" << M.m_states << "}, {" <<
		M.m_alphabet << "}, transitions, " << M.m_initialState << ", {" << M.m_finalStates << "})\n";
	os << "transitions:\n";
	std::map<std::pair<char, RegEx>, std::vector<std::string>> mapOfArguments;

	for (Transition t : M.m_transitions)
	{
		if (mapOfArguments.find(t.GetArguments()) != mapOfArguments.end())
		{

			mapOfArguments[t.GetArguments()].push_back(t.GetResult());
		}
		else
		{
			mapOfArguments.insert({ t.GetArguments(), {t.GetResult()} });
		}

	}
	if (!mapOfArguments.empty())
	{
		for (auto t : mapOfArguments)
		{
			os << "transition(" << t.first.first << ", " << t.first.second << ") = { ";
			for (auto i : t.second)
			{
				os << i << " ";
			}
			os << "}\n";
		}
	}
	else
	{
		os << "empty map!\n";
	}
	return os;
}
