#include "gerex.h"

bool DFA::match(std::string& text) {
	State* state;

	state = this->start_state;
	for (char& c : text) {
		state = state->moves[c];
		if (state == nullptr) {
			return false;
		}
	}

	for (State* final : this->final_states) {
		if (state == final) {
			return true;
		}
	}

	return false;
}