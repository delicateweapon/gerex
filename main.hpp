#pragma once

#include <string>
#include <unordered_map>
#include <vector>

class NFA {
public:
  class State {
  public:
    std::vector<State *> eps_moves;
    std::unordered_map<char, State *> moves;
    size_t id;

    State(void);

    void add_eps_move(State *next);

    static std::size_t count;
    static std::size_t eps_moves_count;
  };

  State *start;
  State *end;

  NFA(void);
  NFA(char symbol);

  enum op : int {
    LPAREN,
    RPAREN,
    CLOSURE,
    UNION,
    CONCAT,
  };

  static int precedence(op &o);

  static NFA *form_closure(NFA *nfa);
  static NFA *form_union(NFA *nfa1, NFA *nfa2);
  static NFA *form_concat(NFA *nfa1, NFA *nfa2);

  static NFA *parse_from_string(std::string &expr);

private:
  static std::vector<NFA *> nfas;
  static std::vector<op> ops;

  static void ops_collapse(op o);
};

class DFA {};
