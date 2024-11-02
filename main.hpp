#pragma once

#include <string>
#include <vector>

class NFA {
public:
  class State {
  public:
    std::vector<State *> eps_moves;
    char symbol;
    State *symbol_next;
    size_t id;

    State(void);

    void add_eps_move(State *next);
    void print(void);
    void print_recurse(void);

    static std::size_t count;
    static std::size_t eps_moves_count;

  private:
      bool printed;
      bool recurse_printed;
  };

  State *start;
  State *end;

  NFA(bool init_state);
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

  static void reset_counts(void);

  void print(void);

private:
  static std::vector<NFA *> nfas;
  static std::vector<op> ops;

  static void ops_collapse(op o);
};

class DFA {};
