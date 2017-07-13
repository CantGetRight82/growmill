#pragma once

#include <string>
#include <vector>
#include <iostream>



using namespace std;


class ExpressionPart {
	public:
		string var;
		string nr;
		bool negate;
		ExpressionPart(string var, string nr="", bool negate = false) : var(var), nr(nr), negate(negate) {
		}

		double number() {
			double result = strtod(nr.c_str(), NULL);
			if(negate) {
				return -result;
			}
			return result;
		}

		static vector<ExpressionPart> parse(std::string str) {
			vector<ExpressionPart> seq;
			bool neg = false;
			bool factor = false;

			const char* delimeters = " <>*-+";
			for(int i=0; i<str.size(); i++) {
				char c = str[i];
				switch(c) {
					case ' ':
					case '+':
						break;
						
					case '<':
					case '>':
						seq.push_back( ExpressionPart(string(c,1)) );
						break;

					case '-':
						neg = !neg;
						break;

					case '*':
						factor = true;
						break;

					default:
						size_t end = str.find_first_of(delimeters, i);
						if(end == string::npos) {
							end = str.size();
						}
						size_t len = end-i;
						string sub = str.substr(i,len);
						if(isdigit(sub[0])) {
							//if factor, this number should be part of term
							if(factor && seq.size() && seq.back().var.size()) {
								seq.back().nr = sub;
								if(neg) {
									seq.back().negate = !seq.back().negate;
								}
								factor = false;
							} else {
								//lone number, is a constant
								seq.push_back( ExpressionPart("", sub, neg ) );
							}

						} else {
							seq.push_back( ExpressionPart(sub, "1", neg) );
						}
						i += len-1;
						neg = false;
				}
			}
			return seq;
		}
};




