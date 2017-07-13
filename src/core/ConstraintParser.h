#pragma once

#include "../../vendor/kiwi/kiwi/kiwi.h"

#include "SymbolPos.h"

#include <string>
#include <iostream>

using namespace std;



class Token {
	public:
		char c;
		string info;
		Token(char c) : c(c), info("") {
		}
};

class ConstraintParser {
	public:
		static kiwi::Constraint parse(std::string str) {
			string buf;
			vector<Token> seq;
			for(int i=0; i<str.size(); i++) {
				char c = str[i];
				switch(c) {
					case '<':
					case '>':
					case '*':
					case '/':
					case '+':
					case '-':
						seq.push_back(Token(c));
						break;

					default:
						if(!seq.size() || seq.back().c != 's') {
							seq.push_back(Token('s'));
						}
						seq.back().info += c;

						break;
				}
			}
			for(Token tok : seq) {
				cout<<tok.c<<":"<<tok.info<<endl;
			}


			double constant = 0;

			kiwi::Expression exp(constant);
			return kiwi::Constraint(exp, kiwi::OP_EQ, kiwi::strength::required );
		}
};

