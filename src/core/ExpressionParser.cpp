#include "ExpressionParser.h"

using std::string;
using std::vector;

using namespace kiwi;

void ExpressionParser::parse(string lhs, string rhs, Solver& solver, VarProvider& provider) {

	Variable from = provider.getVar( lhs );
	vector<ExpressionPart> parts = ExpressionPart::parse( rhs );

	if(parts.size() == 1 && parts[0].var == "") {
		if(!solver.hasEditVariable(from)) {
			solver.addEditVariable( from, strength::strong );
		}
		solver.suggestValue( from, parts[0].number() );
	} else {
		RelationalOperator op = OP_EQ;
		double strength = strength::strong;

		double constant = 0;
		vector<Term> terms;
		terms.push_back( Term(from, -1) );
		for(int i=0; i<parts.size(); i++) {
			if(parts[i].var.size()) {
				switch(parts[i].var[0]) {
					case '<': op = OP_LE; break;
					case '>': op = OP_GE; break;
                    case '!':
                        strength = strength::required;
                        break;
					default:
							  terms.push_back( Term( provider.getVar(parts[i].var), parts[i].number() ) );
							  break;
				}
			} else {
				constant = parts[i].number();
			}
		}

		Constraint c = Constraint( Expression(terms,constant), op, strength);
//		debug::dump(c);
		solver.addConstraint(c);
	}
}
