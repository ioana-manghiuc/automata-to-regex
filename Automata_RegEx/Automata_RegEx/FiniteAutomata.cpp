#include "FiniteAutomata.h"
#include <fstream>
#include <random>
#include <sstream>

FiniteAutomata::FiniteAutomata()
{
	ReadTransitionsFromFile();
	UniformFiniteAutomata();
}

void FiniteAutomata::ReadTransitionsFromFile()
{
	std::ifstream input("automata.txt");

	if (input.is_open())
	{
		std::string states;
		std::getline(input, states);
		std::istringstream stream(states);
		m_states = { std::istream_iterator<std::string>{stream},
					std::istream_iterator<std::string>{} };

		std::getline(input, m_alphabet);

		std::string finStates;
		std::getline(input, finStates);
		std::istringstream finstream(finStates);
		m_finalStates = { std::istream_iterator<std::string>{finstream},
							std::istream_iterator<std::string>{} };

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

void FiniteAutomata::UniformFiniteAutomata()
{
	std::string initState( 1, 'i' );
	std::string finalState(1, 'f' );
	RegEx rgx(lambda);

	std::vector<Transition> newTransitions = m_transitions;

	if (!TransitionExists(initState, lambda, m_initialState, newTransitions))
	{
		Transition newTransition(initState, rgx, m_initialState);
		newTransitions.push_back(newTransition);
	}

	for (const std::string& state: m_finalStates)
	{
		if (!TransitionExists(state, lambda, finalState, newTransitions))
		{
			Transition newTransition(state, rgx, finalState);
			newTransitions.push_back(newTransition);
		}
	}

	if (std::find(m_states.begin(), m_states.end(), initState) == m_states.end() &&
		std::find(m_states.begin(), m_states.end(), finalState) == m_states.end())
	{
		m_states.push_back(initState);
		m_states.push_back(finalState);
	}

	m_transitions = newTransitions;
	m_initialState = initState;
	m_finalStates = { finalState };
}

const std::vector<Transition>& FiniteAutomata::GetTransitions() const
{
	return m_transitions;
}

bool FiniteAutomata::TransitionExists(std::string leftState, std::string symbol, std::string rightState, std::vector<Transition> transitions)
{
	for (Transition t : transitions)
	{
		if (t.GetArguments().first == leftState 
			&& t.GetArguments().second.GetStringPattern() == symbol
			&& t.GetResult() == rightState)
		{
			return true;
		}
	}
	return false;
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
	m_states.erase(std::remove(m_states.begin(), m_states.end(), state), m_states.end());

	m_transitions.erase(std::remove_if(m_transitions.begin(), m_transitions.end(),
		[state, this](const Transition& t) {
			return (t.GetArguments().first == state || t.GetResult() == state);
		}), m_transitions.end());
}

std::string FiniteAutomata::RandomState()
{
	std::random_device RD; 
	std::mt19937 engine(RD()); 
	std::uniform_int_distribution<> distr(0, m_states.size() - 1);

	std::string randState;
	do
	{
		randState = m_states[distr(engine)];
	} while (randState == m_initialState || randState == " " ||
		std::find(m_finalStates.begin(), m_finalStates.end(), randState) != m_finalStates.end());

	return randState;
}

std::multimap<std::string, std::string> FiniteAutomata::GetStatesConnectedVia(const std::string& state) const
{
	std::multimap<std::string, std::string> connectedStates{};
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
				std::cout << "right: " << onRight.GetArguments().first << "\nleft: " << onLeft.GetResult() << "\n\n";
				connectedStates.insert(std::make_pair(onRight.GetArguments().first,
					onLeft.GetResult()));
			}
		}
	}

	for (const auto& pair : connectedStates)
	{
		std::cout << pair.first << " -> " << state << " -> " << pair.second << "\n";
	}
	std::cout << "\n";

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
	std::cout << "kk: " << kk << "\n";
	if (kk.GetStringPattern() != lambda)
	{
		kk = kk.Concatenation(kq);
	}
	std::cout << "kkkq: " << kk << "\n";
	pk = pk.Concatenation(kk);
	std::cout << "pkkkkq: " << pk << "\n";
	pq = pq.Union(pk);
	std::cout << "pq: " << pq << "\n";
	return pq;
}

void FiniteAutomata::ReplaceLabels()
{
	std::string randomState = RandomState();
	std::multimap<std::string, std::string>  paths = GetStatesConnectedVia(randomState);

	if (!paths.empty())
	{
		for (const auto& path : paths)
		{
			RegEx rgx = ComputeRegexForStates(path.first, randomState, path.second);
			rgx.RemoveLambdas();
			std::cout << "\nREGEX FORM: " << rgx.GetStringPattern() << "\n\n";

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

	} while (m_states.size() > 2);

	if (!m_transitions.empty())
	{
		RegEx rgx = m_transitions[0].GetArguments().second;
		rgx.SetRegexPattern(rgx.GetStringPattern());
		return rgx;
	}
	else
	{
		return RegEx();
	}
}


std::ostream& operator<<(std::ostream& os, const FiniteAutomata& M)
{
	os << "M = ({";
	for (const std::string& state : M.m_states)
	{
		if (state == M.m_states[M.m_states.size() - 1])
		{
			os << state;
		}
		else
		{
			os << state << " ";
		}
	}
	os << "}, {" <<
		M.m_alphabet << "}, transitions, " << M.m_initialState << ", ";
	if (M.m_finalStates.size() == 1)
	{
		os << M.m_finalStates[0];
	}
	else
	{
		os << "{ ";
		for (const std::string& state : M.m_finalStates)
		{
			os << state << " ";
		}
		os << "}";
	}
	os << ")\n";
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
	os << "\n";
	return os;
}
