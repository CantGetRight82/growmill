#pragma once

#include <string>
#include <vector>
#include <iostream>

class ExpressionPart {
	public:
		std::string var;
		std::string nr;
    
        bool negate;
        bool divide;
        ExpressionPart() : var(""), nr("1"), negate(false), divide(false) {
        
        }

		double number();
        static std::vector<ExpressionPart> parse(std::string str);
};




