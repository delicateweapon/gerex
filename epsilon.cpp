#include "gerex.h"


std::vector<std::vector<NFA::State*>*> NFA::eps_closures;

std::vector<NFA::State*>* NFA::eps_closure(State* s) {
	if (s->eps_closure_generated) {
		return eps_closures[s->id];
	}

	auto result = new std::vector<State*>;
	
	for (State* e : s->eps_moves) {
		result->push_back(e);
		auto e_c = NFA::eps_closure(e);
		result->insert(result->end(), e_c->begin(), e_c->end());
	}

	s->eps_closure_generated = true;
	eps_closures[s->id] = result;
	return result;
}

void NFA::eps_closures_generate() {
	NFA::eps_closures.reserve(NFA::State::count);
	NFA::eps_closures.resize(NFA::State::count, nullptr);

	auto i = 0;
	while (i < NFA::State::count) {
		NFA::eps_closures.emplace_back(NFA::eps_closure(NFA::state_table[i]));
		i++;
	}
}
