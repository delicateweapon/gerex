// gerex.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <iostream>
#include <vector>
#include <unordered_map>
#include <cstdint>
#include <algorithm>

class NFA {
public:
	class State {
	public:
		std::size_t id;
		std::vector<State*> eps_moves;
		State* symbol_next;
		char symbol;
		bool eps_closure_generated = false;

		State();

		void add_eps_move(State* next);

		static std::size_t count;
		static std::size_t eps_moves_count;
	};

	State* begin;
	State* end;

	NFA(bool init_states);
	NFA(char symbol);

	static std::vector<char> symbols;
	static void symbols_print();

	static NFA* apply_union(NFA* nfa1, NFA* nfa2);
	static NFA* apply_concat(NFA* nfa1, NFA* nfa2);
	static NFA* apply_closure(NFA* nfa);

	enum op {
		LPAREN,
		RPAREN,
		UNION,
		CONCAT,
		CLOSURE,
	};

	static std::uint8_t precedence(op& o);

	static NFA* parse_from_expr(std::string& expr);

	static void state_table_print();

	static std::vector<std::vector<State*>*> eps_closures;
	static void eps_closures_generate();
	static void eps_closures_print();

private:
	static std::vector<NFA*> nfas;
	static std::vector<op> ops;

	static void ops_collapse(op o);

	static std::vector<State*> state_table;

	static std::vector<State*>* eps_closure(State* s);
};

class DFA {
public:
	class State {
	public:
		std::unordered_map<char, State*> moves;
		std::size_t id;

		State();

	private:
		static std::size_t count;
	};

	std::vector<State*> states;
	State* start_state;
	std::vector<State*> final_states;

	DFA(NFA *nfa);

	bool match(std::string& text);
	void print();

private:
	static std::vector<NFA::State*> considered_states;
};
