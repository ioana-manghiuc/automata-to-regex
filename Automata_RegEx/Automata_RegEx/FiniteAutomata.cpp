#include "FiniteAutomata.h"
#include <fstream>
#include <random>

FiniteAutomata::FiniteAutomata()
{
	ReadTransitionsFromFile();
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

const std::vector<Transition>& FiniteAutomata::GetTransitions() const
{
	return m_transitions;
}

Transition FiniteAutomata::GetTransitionOf(const std::string& left, const std::string& right) const
{
	auto it = std::find_if(m_transitions.begin(), m_transitions.end(), [&](const Transition& t) {
		return t.GetArguments().first == left && t.GetResult() == right;
		});

	if (it != m_transitions.end()) {
		return *it;
	}
	else {
		throw std::runtime_error("Transition not found");
	}
}

int FiniteAutomata::GetStateSize()
{
	return std::count_if(m_states.begin(), m_states.end(), [](char c) {
		return c != ' ';
		});
}

bool FiniteAutomata::TransitionExists(std::string leftState, std::string symbol, std::string rightState, std::vector<Transition> transitions)
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

bool FiniteAutomata::TransitionExists(std::string leftState, std::string rightState, std::vector<Transition> transitions)
{
	for (Transition t : transitions)
	{
		if (t.GetArguments().first == leftState && t.GetResult() == rightState)
		{
			return true;
		}
	}
	return false;
}

void FiniteAutomata::UniformFiniteAutomata()
{
	char newInitialState = 'i';
	char newFinalState = 'f';
	std::string initState(1, newInitialState);
	std::string finalState(1, newFinalState);
	RegEx rgx(lambda);

	std::vector<Transition> newTransitions = m_transitions;

	if (!TransitionExists(initState, lambda, std::string(1, m_initialState), newTransitions))
	{
		Transition newTransition(initState, rgx, std::string(1, m_initialState));
		newTransitions.push_back(newTransition);
	}

	for (char c : m_finalStates)
	{
		std::string chr(1, c);
		if (c != ' ' && !TransitionExists(chr, lambda, finalState, newTransitions))
		{
			Transition newTransition(chr, rgx, finalState);
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

void FiniteAutomata::RemoveTransition(const Transition& t)
{
	m_transitions.erase(
		std::remove_if(m_transitions.begin(), m_transitions.end(),
			[&t](const Transition& transition) {
				return transition == t;
			}),
		m_transitions.end()
	);
}

void FiniteAutomata::RemoveState(const std::string& state)
{
	m_states.erase(std::remove(m_states.begin(), m_states.end(), state[0]), m_states.end());

	m_transitions.erase(std::remove_if(m_transitions.begin(), m_transitions.end(),
		[state, this](const Transition& t) {
			return (t.GetArguments().first == state || t.GetResult() == state) 
				&& (t.GetArguments().first != std::string(1,m_initialState && (t.GetResult() != m_finalStates))) ;
		}), m_transitions.end());
}

std::string FiniteAutomata::RandomState()
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

	return std::string(1, randState);
}

std::map<std::string, std::string> FiniteAutomata::GetStatesConnectedVia(const std::string& state) const
{
	std::map<std::string, std::string> connectedStates{};
	std::vector<Transition> stateOnLeft, stateOnRight;

	auto isStateOnLeft = [&state](const Transition& t) { return t.GetArguments().first == state; };
	auto isStateOnRight = [&state](const Transition& t) { return t.GetResult() == state; };
	std::copy_if(m_transitions.begin(), m_transitions.end(), std::back_inserter(stateOnLeft), isStateOnLeft);
	std::copy_if(m_transitions.begin(), m_transitions.end(), std::back_inserter(stateOnRight), isStateOnRight);


	for (const Transition& onRight : stateOnRight)
	{
		for (const Transition& onLeft : stateOnLeft)
		{
			if (onRight.GetArguments().first != state && onLeft.GetResult() != state)
			{
				connectedStates.insert(std::make_pair(onRight.GetArguments().first,
					onLeft.GetResult()));
			}
		}
	}

	for (const auto& pair : connectedStates)
	{
		std::cout << pair.first << " -> " << state << " -> " << pair.second << "\n";
	}

	return connectedStates;
}

RegEx FiniteAutomata::GetDirectTransitionLabelBetween(const std::string& lhs, const std::string& rhs)
{
	std::vector<Transition> directTransitions;
	auto isDirectTransition = [lhs, rhs](const Transition& t) -> bool
		{
			return t.GetArguments().first == lhs && t.GetResult() == rhs;
		};
	std::copy_if(m_transitions.begin(), m_transitions.end(), std::back_inserter(directTransitions), isDirectTransition);

	if (directTransitions.empty())
	{
		return RegEx();
	}
	else if (directTransitions.size() == 1)
	{
		return directTransitions[0].GetArguments().second;
	}
	else
	{
		RegEx rgx;
		for (const Transition& t : directTransitions)
		{
			rgx = rgx.Union(t.GetArguments().second);
			RemoveTransition(t);
		}
		RegEx newPattern{ "(" + rgx.GetStringPattern() + ")" };
		m_transitions.push_back(Transition{ lhs, newPattern, rhs });
		return newPattern;
	}
}

RegEx FiniteAutomata::ComputeRegexForStates(const std::string& p, const std::string& k, const std::string& q)
{
	auto pq = GetDirectTransitionLabelBetween(p, q);
	auto pk = GetDirectTransitionLabelBetween(p, k);
	auto kk = GetDirectTransitionLabelBetween(k, k);
	auto kq = GetDirectTransitionLabelBetween(k, q);

	kk = kk.KleeneStar();
	if (kk.GetStringPattern() != lambda)
	{
		kk = kk.Concatenation(kq);
	}
	pk = pk.Concatenation(kk);
	pq = pq.Union(pk);
	return pq;
}

void FiniteAutomata::ReplaceLabels()
{
	std::string randomState = RandomState();
	std::map<std::string, std::string>  paths = GetStatesConnectedVia(randomState);

	if (!paths.empty())
	{
		for (const auto& path : paths)
		{
			RegEx rgx = ComputeRegexForStates(path.first, randomState, path.second);
			rgx.RemoveLambdas();
			std::cout << "\nregex form:" << rgx.GetStringPattern() << "\n\n";

			auto existingTransition = std::find_if(m_transitions.begin(), m_transitions.end(), 
				[&](const Transition& t)
				{
					return t.GetArguments().first == path.first && t.GetResult() == path.second;
				});

			if (existingTransition != m_transitions.end())
			{
				existingTransition->SetRegexLabel(rgx);
			}
			else
			{
				m_transitions.push_back(Transition{ path.first, rgx, path.second });
			}
		}
	}
	
	RemoveState(randomState);
	std::cout << "POST-STATE REMOVAL:\n" << * this;

}

RegEx FiniteAutomata::RegularExpression()
{
	do
	{
		ReplaceLabels();
	} while (GetStateSize() > 2);

	if (!m_transitions.empty())
	{
		return m_transitions[0].GetArguments().second;
	}
	else
	{
		return RegEx();
	}
}


std::ostream& operator<<(std::ostream& os, const FiniteAutomata& M)
{
	os << "M = ({" << M.m_states << "}, {" <<
		M.m_alphabet << "}, transitions, " << M.m_initialState << ", {" << M.m_finalStates << "})\n";
	os << "transitions:\n";

	std::map<std::pair<std::string, RegEx>, std::vector<std::string>> mapOfArguments;

	if (!M.m_transitions.empty())
	{
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
