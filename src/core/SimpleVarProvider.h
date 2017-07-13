#pragma once
#include "VarProvider.h"
#include "../../vendor/kiwi/kiwi/kiwi.h"

#include <string>
#include <map>

class SimpleVarProvider : public VarProvider {
	public:
		std::map<std::string,kiwi::Variable> vars;
		
		SimpleVarProvider() {
		}

		kiwi::Variable getVar(std::string key) {
			if(vars.count(key) == 0) {
				vars[key] = kiwi::Variable(key);
			}
			return vars[key];
		}
};
