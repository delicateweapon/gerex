#include "gerex.h"

#include <stdexcept>

std::uint8_t NFA::precedence(NFA::op& o) {
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
};

std::vector<NFA*> NFA::nfas;
std::vector<NFA::op> NFA::ops;
std::vector<char> NFA::symbols;

NFA::op top;
NFA* nfa1;
NFA* nfa2;

void NFA::ops_collapse(op o) {
	if (ops.size() == 0) {
		return;
	}

	top = ops.back();
	while (precedence(top) > precedence(o)) {
		ops.pop_back();

		switch (top) {
		case LPAREN:
		case RPAREN:
			/* Not happening */
			break;

		case CLOSURE:
			if (nfas.size() < 1) {
				throw std::runtime_error("Invalid Expression, check if '*' is used correctly");
			}
			nfa1 = nfas.back();
			nfas.pop_back();
			nfas.push_back(apply_closure(nfa1));
			break;

		case UNION:
			if (nfas.size() < 2) {
				throw std::runtime_error("Invalid Expression, check if '|' is used correctly");
			}
			nfa1 = nfas.back();
			nfas.pop_back();
			nfa2 = nfas.back();
			nfas.pop_back();
			nfas.push_back(apply_union(nfa2, nfa1));
			break;

		case CONCAT:
			if (nfas.size() < 2) {
				throw std::runtime_error("Invalid Expression");
			}
			nfa1 = nfas.back();
			nfas.pop_back();
			nfa2 = nfas.back();
			nfas.pop_back();
			nfas.push_back(apply_concat(nfa2, nfa1));
			break;
		}

		if (ops.size() == 0) {
			break;
		}
		top = ops.back();
	}
}

NFA* NFA::parse_from_expr(std::string& expr) {
	NFA::nfas.clear();
	NFA::ops.clear();

	NFA::nfas.reserve(8);
	NFA::ops.reserve(8);

	bool push_concat;

	push_concat = false;
	for (char& c : expr) {
		if (std::isalnum(c)) {
			if (push_concat) {
				ops_collapse(op::CONCAT);
				NFA::ops.emplace_back(op::CONCAT);
			}
			NFA::nfas.emplace_back(new NFA(c));
			NFA::symbols.push_back(c);
			push_concat = true;
			continue;
		}

		switch (c) {
		case '*':
			ops_collapse(op::CLOSURE);
			NFA::ops.emplace_back(op::CLOSURE);
			break;

		case '|':
			ops_collapse(op::UNION);
			NFA::ops.emplace_back(op::UNION);
			push_concat = false;
			break;

		case '(':
			if (push_concat) {
				ops_collapse(op::CONCAT);
				NFA::ops.emplace_back(op::CONCAT);
			}
			NFA::ops.emplace_back(op::LPAREN);
			push_concat = false;
			break;

		case ')':
			ops_collapse(op::RPAREN);
			if (ops.size() == 0) {
				throw std::runtime_error("Invalid Expression, check if brackets are placed properly");
			}
			if (ops.back() != op::LPAREN) {
				throw std::runtime_error("Invalid Expression, check if brackets are placed properly");
			}
			NFA::ops.pop_back();
			push_concat = true;
			break;

		default:
			throw std::runtime_error("Invalid Expression");
			break;
		}
	}

	ops_collapse(op::LPAREN);

	if (NFA::ops.size() != 0 || NFA::nfas.size() != 1) {
		throw std::runtime_error("Invalid Expression");
	}

	return nfas.back();
}