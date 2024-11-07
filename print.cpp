#include "gerex.h"

void NFA::symbols_print() {
	std::cout << "Symbols: ";
	for (char& sym : NFA::symbols) {
		std::cout << sym << " ";
	}
	std::cout << "\n";
}

void NFA::state_table_print() {
	auto state = NFA::state_table.begin();

	std::cout << "State\t\t" << "Symbol Move\t\t" << "Epsilon Moves\n\n";
	while (state != NFA::state_table.end()) {
		std::cout << "q" << (*state)->id << "\t\t";

		if ((*state)->symbol_next != nullptr) {
			std::cout << "q" << (*state)->symbol_next->id << " (" << (*state)->symbol << ")\t\t\tNone\t\t\n";
			state++;
			continue;
		}
		std::cout << "None\t\t\t";

		if ((*state)->eps_moves.size() == 0) {
			std::cout << "None\n";
			state++;
			continue;
		}
		for (State* next : (*state)->eps_moves) {
			std::cout << "q" << next->id << ", ";
		}

		std::cout << "\n";

		state++;
	}
}

void NFA::eps_closures_print() {
	std::cout << "State\t\t" << "Eps-Closures\n\n";

	auto i = 0;
	while (i < NFA::State::count) {
		std::cout << "q" << i << "\t\t";
		for (State* s : *NFA::eps_closures[i]) {
			std::cout << "q" << s->id << ", ";
		}
		if (NFA::eps_closures[i]->size() == 0) {
			std::cout << "None";
		}
		std::cout << "\n";
		i++;
	}
}

void DFA::print() {
	std::cout << "Start State: p" << this->start_state->id << "\n";
	std::cout << "Final States : ";

	for (State* s : this->final_states) {
		std::cout << "p" << s->id << " ";
	}
	std::cout << "\n\n";

	std::cout << "State\t\t";
	for (char& sym : NFA::symbols) {
		std::cout << sym << "\t\t";
	}
	std::cout << "\n\n";

	for (State* s : this->states) {
		std::cout << "p" << s->id << "\t\t";

		for (char& sym : NFA::symbols) {
			auto n = s->moves[sym];
			if (n == nullptr) {
				std::cout << "None\t\t";
			}
			else {
				std::cout << "p" << n->id << "\t\t";
			}
		}

		std::cout << "\n";
	}
}