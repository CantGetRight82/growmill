#pragma once

#include "../../vendor/kiwi/kiwi/kiwi.h"
#include "VarProvider.h"
#include "ExpressionPart.h"


class ExpressionParser {
	public:
    static void parse(std::string lhs, std::string rhs, kiwi::Solver& solver, VarProvider& provider);
};
