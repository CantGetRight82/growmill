#pragma once

#include "../../vendor/kiwi/kiwi/kiwi.h"
#include <string>

class VarProvider {
	public:
		virtual kiwi::Variable getVar(std::string key) = 0;
};

