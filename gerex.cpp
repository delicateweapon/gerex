// gerex.cpp : Defines the entry point for the application.
//

#include "gerex.h"

int main() {
	std::string expr, text;
	NFA* nfa;
	DFA* dfa;
	bool result;

	try {
		expr = "(a|b)*c";
		nfa = NFA::parse_from_expr(expr);
		NFA::eps_closures_generate();
		dfa = new DFA(nfa);

		text = "ababc";
		result = dfa->match(text);

		std::cout << "hehe, that's a valid expression\n\n";

		std::cout << "Expression: " << expr << "\n";
		std::cout << "Text: " << text << "\n";
		std::cout << "Match: " << (result ? "true" : "false") << "\n\n";

		std::cout << "Total State Count: " << NFA::State::count << "\n";
		std::cout << "Total Epsilon Moves: " << NFA::State::eps_moves_count << "\n";
		NFA::symbols_print();
		std::cout << "Entry State: q" << nfa->begin->id << "\n";
		std::cout << "Exit State: q" << nfa->end->id << "\n\n";
		NFA::state_table_print();
		std::cout << "\n\n";
		NFA::eps_closures_print();
		std::cout << "\n\n";
		dfa->print();

	}
	catch (std::exception& e) {
		std::cerr << e.what() << "\n";
	}
	return 0;
}
