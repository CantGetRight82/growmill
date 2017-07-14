#pragma once

#include <string>
#include <vector>
#include <iostream>

class ExpressionPart {
	public:
		std::string var;
		std::string nr;
		bool negate;
        ExpressionPart(std::string var, std::string nr="", bool negate = false) : var(var), nr(nr), negate(negate) {
		}

		double number();
        static std::vector<ExpressionPart> parse(std::string str);
};




