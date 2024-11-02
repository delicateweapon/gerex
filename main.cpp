#include "main.hpp"

#include <iostream>
#include <stdexcept>

int main(int argc, char **argv) {

  if (argc < 2) {
    std::cerr << "Ummm, please enter an expression\n";
    return -1;
  }

  try {
    std::string expr = argv[1];
    NFA *nfa = NFA::parse_from_string(expr);
    std::cout << "hehe, i can see that's a valid expression\n\n";
    std::cout << "Total States: " << NFA::State::count << "\n";
    std::cout << "Total Epsilon Moves: " << NFA::State::eps_moves_count << "\n\n";
    nfa->print();
    nfa->reset_counts();
  } catch (std::exception &e) {
    std::cerr << e.what() << std::endl;
    return -1;
  }

  // std::vector<std::string> exprs = {
  //     "(a|b)*c", "abc", "ab*c", "a*b*c", "abcd", "abc*d", "ab*c*d*",
  // };

  // try {
  //   NFA *nfa;
  //   for (std::string &expr : exprs) {

  //     nfa = NFA::parse_from_string(expr);
  //     (void)nfa;

  //     std::cout << "Expr: " << expr << "\n";
  //     std::cout << "Total NFA state count: " << NFA::State::count << "\n";
  //     std::cout << "Total NFA epsilon moves count: "
  //               << NFA::State::eps_moves_count << "\n\n";
  //     // nfa->print();
  //     // std::cout << "\n------------------------------\n";
  //     // std::cout << std::endl;

  //     NFA::reset_counts();
  //   }
  // } catch (std::exception &e) {
  //   std::cout << e.what() << std::endl;
  // }

  return 0;
}

std::size_t NFA::State::count = 0;
std::size_t NFA::State::eps_moves_count = 0;

void NFA::reset_counts(void) {
  NFA::State::count = 0;
  NFA::State::eps_moves_count = 0;
}

NFA::State::State(void) {
  this->id = State::count;
  this->symbol = 0;
  this->symbol_next = nullptr;
  this->printed = false;
  this->recurse_printed = false;

  State::count++;
}

void NFA::State::add_eps_move(State *next) {
  this->eps_moves.push_back(next);
  State::eps_moves_count++;
}

NFA::NFA(bool init_state) {
  if (init_state) {

    this->start = new State();
    this->end = new State();

  } else {
    this->start = nullptr;
    this->end = nullptr;
  }
}

NFA::NFA(char symbol) {
  this->start = new State();
  this->end = new State();
  this->start->symbol = symbol;
  this->start->symbol_next = this->end;
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
  NFA *result = new NFA(true);

  result->start->add_eps_move(result->end);
  result->start->add_eps_move(nfa->start);
  nfa->end->add_eps_move(result->end);
  nfa->end->add_eps_move(nfa->start);

  return result;
}

NFA *NFA::form_union(NFA *nfa1, NFA *nfa2) {
  NFA *result = new NFA(true);

  result->start->add_eps_move(nfa1->start);
  result->start->add_eps_move(nfa2->start);
  nfa1->start->add_eps_move(result->end);
  nfa2->start->add_eps_move(result->end);

  return result;
}

NFA *NFA::form_concat(NFA *nfa1, NFA *nfa2) {
  NFA *result = new NFA(false);
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
      if (nfas.size() < 1) {
        throw std::runtime_error(
            "Invalid expression, check if '*' is used correctly");
      }
      nfa1 = NFA::nfas.back();
      NFA::nfas.pop_back();

      NFA::nfas.emplace_back(NFA::form_closure(nfa1));
      break;

    case CONCAT:
      if (nfas.size() < 2) {
        throw std::runtime_error("Invalid expression");
      }
      nfa1 = NFA::nfas.back();
      NFA::nfas.pop_back();
      nfa2 = NFA::nfas.back();
      NFA::nfas.pop_back();

      NFA::nfas.emplace_back(NFA::form_concat(nfa2, nfa1));
      break;

    case UNION:
      if (nfas.size() < 2) {
        throw std::runtime_error(
            "Invalid expression, check if '|' is used correctly");
      }
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
  NFA *result;

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
      // NFA::ops_collapse(op::LPAREN);
      if (push_concat) {
        NFA::ops_collapse(op::CONCAT);
        NFA::ops.push_back(op::CONCAT);
      }
      NFA::ops.push_back(op::LPAREN);
      push_concat = false;
      break;

    case ')':
      NFA::ops_collapse(op::RPAREN);
      if (NFA::ops.back() != op::LPAREN || NFA::ops.size() == 0) {
        throw std::runtime_error("Invalid expression: missing '('");
      }
      NFA::ops.pop_back();
      push_concat = true;
      break;

    default:
      std::runtime_error("Invalid expression");
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

void NFA::State::print(void) {
  if (this->printed) {
    return;
  }
  this->printed = true;

  for (State *state : this->eps_moves) {
    std::cout << "[" << this->id << "->" << state->id << "]\t";
  }
  if (this->eps_moves.size() > 0) {
    std::cout << "\n";
  }

  if (this->symbol_next != nullptr) {
    std::cout << "For Symbol '" << this->symbol << "' : [" << this->id << "->"
              << this->symbol_next->id << "]\n";
  }
}

void NFA::State::print_recurse(void) {
  if (this->recurse_printed) {
    return;
  }
  this->recurse_printed = true;

  this->print();

  for (State *state : this->eps_moves) {
    state->print_recurse();
  }

  if (this->symbol_next != nullptr) {
    this->symbol_next->print_recurse();
  }
}

void NFA::print(void) { this->start->print_recurse(); }
