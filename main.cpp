#include "main.hpp"

#include <iostream>

std::size_t NFA::State::count = 0;
std::size_t NFA::State::eps_moves_count = 0;

NFA::State::State(void) {
  this->id = State::count;

  State::count++;
}

void NFA::State::add_eps_move(State *next) {
  this->eps_moves.push_back(next);
  State::eps_moves_count++;
}

NFA::NFA(void) {
  this->start = new State();
  this->end = new State();
}

NFA::NFA(char symbol) {
  this->start = new State();
  this->end = new State();
  this->start->moves[symbol] = this->end;
}

int NFA::precedence(op &o) {
  switch (o) {
  case LPAREN:
  case RPAREN:
    return 0;
  case UNION:
    return 1;
  case CONCAT:
    return 2;
  case CLOSURE:
    return 3;
  }
}

NFA *NFA::form_closure(NFA *nfa) {
  NFA *result = new NFA();

  result->start->add_eps_move(result->end);
  result->start->add_eps_move(nfa->start);
  nfa->end->add_eps_move(result->end);
  nfa->end->add_eps_move(nfa->start);

  return result;
}

NFA *NFA::form_union(NFA *nfa1, NFA *nfa2) {
  NFA *result = new NFA();

  result->start->add_eps_move(nfa1->start);
  result->start->add_eps_move(nfa2->start);
  nfa1->start->add_eps_move(result->end);
  nfa2->start->add_eps_move(result->end);

  return result;
}

NFA *NFA::form_concat(NFA *nfa1, NFA *nfa2) {
  NFA *result = new NFA();
  nfa1->end->add_eps_move(nfa2->start);

  result->start = nfa1->start;
  result->end = nfa2->end;

  return result;
}

std::vector<NFA *> NFA::nfas;
std::vector<NFA::op> NFA::ops;

void NFA::ops_collapse(op o) {
  if (NFA::ops.size() == 0) {
    return;
  }

  op &top = NFA::ops.back();

  NFA *nfa1, *nfa2;
  while (NFA::precedence(top) > NFA::precedence(o)) {
    (void)NFA::ops.pop_back();

    switch (top) {
    case LPAREN:
    case RPAREN:
      // not happening
      break;

    case CLOSURE:
      nfa1 = NFA::nfas.back();
      NFA::nfas.pop_back();

      NFA::nfas.emplace_back(NFA::form_closure(nfa1));
      break;

    case CONCAT:
      nfa1 = NFA::nfas.back();
      NFA::nfas.pop_back();
      nfa2 = NFA::nfas.back();
      NFA::nfas.pop_back();

      NFA::nfas.emplace_back(NFA::form_concat(nfa2, nfa1));
      break;

    case UNION:
      nfa1 = NFA::nfas.back();
      NFA::nfas.pop_back();
      nfa2 = NFA::nfas.back();
      NFA::nfas.pop_back();

      NFA::nfas.emplace_back(NFA::form_union(nfa1, nfa2));
      break;
    }

    top = NFA::ops.back();
  }
}

NFA *NFA::parse_from_string(std::string &expr) {
  NFA *result = new NFA();

  bool push_concat = false;
  size_t i = 0;
  (void)i;

  for (char &c : expr) {
    if (std::isalnum(c)) {
      if (push_concat) {
        NFA::ops_collapse(op::CONCAT);
        NFA::ops.push_back(op::CONCAT);
      }
      NFA::nfas.emplace_back(new NFA(c));
      push_concat = true;
    }

    switch (c) {
    case '*':
      /* previously pushed nfa must be concatable for closure to be applied */
      if (!push_concat) {
        throw std::runtime_error("Invalid expression");
      }
      NFA::ops_collapse(op::CLOSURE);
      NFA::ops.push_back(op::CLOSURE);
      break;

    case '|':
      NFA::ops_collapse(op::UNION);
      NFA::ops.push_back(op::UNION);
      push_concat = false;
      break;

    case '(':
      NFA::ops_collapse(op::UNION);
      if (push_concat) {
        NFA::ops_collapse(op::CONCAT);
        NFA::ops.push_back(op::CONCAT);
      }
      NFA::ops.push_back(op::LPAREN);
      push_concat = false;
      break;

    case ')':
      NFA::ops_collapse(op::RPAREN);
      if (NFA::ops.back() != op::LPAREN) {
        throw std::runtime_error("Invalid expression");
      }
      NFA::ops.pop_back();
      push_concat = true;
      break;
    }

    ++i;
  }

  NFA::ops_collapse(op::LPAREN);

  if (NFA::nfas.size() != 1 || NFA::ops.size() != 0) {
    throw std::runtime_error("Invalid expression");
  }

  result = nfas.back();
  nfas.pop_back();
  return result;
}

int main(void) {
  std::string expr = "(a|b)*c";
  NFA *nfa;
  (void)nfa;

  try {
    nfa = NFA::parse_from_string(expr);
  } catch (std::exception &e) {
    std::cerr << e.what() << "\n";
  }

  std::cout << "Total NFA state count: " << NFA::State::count << "\n";
  std::cout << "Total NFA epsilon moves count: " << NFA::State::eps_moves_count
            << "\n";

  return 0;
}
