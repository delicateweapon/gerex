#include "gerex.h"

std::size_t NFA::State::count = 0;
std::size_t NFA::State::eps_moves_count = 0;

std::vector<NFA::State*> NFA::state_table;

NFA::State::State() {
	this->id = NFA::State::count;
	NFA::State::count++;

	this->symbol = 0;
	this->symbol_next = nullptr;
	(void)this->eps_moves;
	this->eps_closure_generated = false;

	NFA::state_table.push_back(this);
}

void NFA::State::add_eps_move(State* next) {
	this->eps_moves.push_back(next);
	NFA::State::eps_moves_count++;
}

NFA::NFA(bool init_states) {
	if (init_states) {
		this->begin = new State();
		this->end = new State();
	}
	else {
		this->begin = nullptr;
		this->end = nullptr;
	}
}

NFA::NFA(char symbol) {
	this->begin = new State();
	this->end = new State();

	this->begin->symbol = symbol;
	this->begin->symbol_next = this->end;
};

NFA* NFA::apply_closure(NFA* nfa) {
	NFA* result = new NFA(true);

	result->begin->add_eps_move(nfa->begin);
	result->begin->add_eps_move(result->end);
	nfa->end->add_eps_move(nfa->begin);
	nfa->end->add_eps_move(result->end);

	return result;
}

NFA* NFA::apply_union(NFA* nfa1, NFA* nfa2) {
	NFA* result = new NFA(true);

	result->begin->add_eps_move(nfa1->begin);
	nfa1->end->add_eps_move(result->end);
	result->begin->add_eps_move(nfa2->begin);
	nfa2->end->add_eps_move(result->end);

	return result;
}

NFA* NFA::apply_concat(NFA* nfa1, NFA* nfa2) {
	nfa1->end->add_eps_move(nfa2->begin);

	NFA* result = new NFA(false);
	result->begin = nfa1->begin;
	result->end = nfa2->end;
	return result;
}
