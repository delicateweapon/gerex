#include "gerex.h"

std::size_t DFA::State::count = 0;

DFA::State::State() {
	this->id = DFA::State::count;
	DFA::State::count++;
	(void)this->moves;
}

std::vector<NFA::State*> DFA::considered_states;

DFA::DFA(NFA* nfa) {
	DFA::considered_states.push_back(nfa->begin);

	NFA::State* considered_state;
	auto i = 0;
	while (i < considered_states.size()) {
		if (this->states.size() != considered_states.size()) {
			this->states.emplace_back(new State());
		}

		considered_state = considered_states[i];
		if (considered_state == nfa->end) {
			this->final_states.emplace_back(this->states[i]);
		}

		auto j = 1;
		for (char& sym : NFA::symbols) {
			for (NFA::State* s : *NFA::eps_closures[considered_state->id]) {
				if (s->symbol != sym) {
					continue;
				}

				auto next = std::find(considered_states.begin(), considered_states.end(), s->symbol_next);
				if (next == considered_states.end()) {
					DFA::considered_states.emplace_back(s->symbol_next);
					this->states.emplace_back(new State());
					this->states[i]->moves[sym] = this->states[i + j];
					j++;
				}
				else {
					auto index = std::distance(considered_states.begin(), next);
					this->states[i]->moves[sym] = this->states[index];
				}

				break;
			}
		}

		i++;
	}

	this->start_state = this->states[0];
}