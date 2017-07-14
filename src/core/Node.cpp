#include "Node.h"

#include <math.h>
#include "ExpressionParser.h"

using namespace kiwi;

class RefVar {
	public:
		string ref;
		string key;
		RefVar(string ref, string key) : ref(ref), key(key) {
		}
};

Node* Node::zero = NULL;

bool Node::has(string key) {
	return atts.count(key);
}

void Node::initVars() {
	string keys[] = { "top", "right", "bottom", "left", "width", "height" };
	for(string key : keys) {
		vars[key] = Variable(key);
	}
}

void Node::constrain(Solver* solver) {
	solver->addConstraint( vars["width"] == vars["right"] - vars["left"] );
	solver->addConstraint( vars["height"] == vars["bottom"] - vars["top"] );
	if(!parent) {
		addStay(solver, "top", 0);
		addStay(solver, "left", 0);
	}

	string keys[] = {
		"top", "right", "bottom", "left", "width", "height"
	};

	bool side = atts["flow"] == "side";
	for(string key : keys) {
		if(atts.count(key)) {
			string val = atts[key];
			if(val.find("out") != std::string::npos) {
				vector<string> parts = split(val," ");
				assert(parts.size() == 2);

				if(key == "top") {
					if(prev) {
						val = "prev.bottom + "+parts[1];
					} else {
						val = "p.top + "+parts[1];
					}
				} else if(key == "left") {
					if(prev) {
						val = "prev.right + "+parts[1];
					} else {
						val = "p.left + "+parts[1];
					}
				} else if(key == "right") {
					if(next) {
						val = "next.left - "+parts[1];
					} else {
						val = "p.right - "+parts[1];
					}
				} else if(key == "bottom") {
					if(next) {
						val = "next.top - "+parts[1];
					} else {
						val = "p.bottom - "+parts[1];
					}
				}
			}
			ExpressionParser::parse( key, val, *solver, *this);
		}
	}

	for( Node* n : subs) {
		n->constrain( solver );
	}

}

void Node::addStay(Solver* solver, string key, float val) {
	solver->addEditVariable( vars[key], strength::strong );
	solver->suggestValue( vars[key], val );
}

int Node::atti(string key) {
	return atoi( atts[key].c_str() );
}

Color Node::color(string key) {
	string val = atts[key];
	return Color::fromString( val );
	/*
	map<string,uint32_t> namedColors ={
		{ "red", 0xFF0000 },
		{ "green", 0x00FF00 },
		{ "blue", 0x0000FF },
	};
	uint32_t hex;
	if(namedColors.count(val)) {
		hex = namedColors[val];
	} else {
		hex = strtoul(val.c_str(), NULL, 16);
	}

	return Color(
			);
			*/
}

float Node::attf(string key) {
	if(atts.count(key)) {
		return atof( atts[key].c_str() );
	}
	return NAN;
}
Node* Node::clone() {
	Node* result = new Node();
	result->atts = this->atts;

	vector<Node*>::iterator p;
	for(p = subs.begin(); p != subs.end(); p++) {
		result->subs.push_back( *p );
	}
	return result;
}

void Node::renderJSON(std::ostream& to) {
	string q = "\"";

	to<<"{"<<endl;
	to<<q<<"atts"<<q<<": {"<<endl;

	map<string, string>::iterator att;
	for ( att = atts.begin(); att != atts.end(); att++ ) {
		to << q << att->first << q
			<< ':'
			<< q << att->second << q
			<< endl ;
	}
	to<<"},"<<endl;

	to<<q<<"subs"<<q<<": {"<<endl;
	vector<Node*>::iterator p;
	for(p = subs.begin(); p != subs.end(); p++) {
		(*p)->renderJSON(to);
	}


	to<<"}"<<endl;
	to<<"}"<<endl;
}

string Node::ids() {
	if(parent) {
		return parent->ids() + "." + std::to_string(parent->idx(this));
	}
	return "0";
}

int Node::idx(Node* child) {
	for(int i=0; i<subs.size(); i++) {
		if(child == subs[i]) { return i; }
	}
	return -1;
}

float Node::top() { return vars["top"].value(); }
float Node::right() { return vars["right"].value(); }
float Node::bottom() { return vars["bottom"].value(); }
float Node::left() { return vars["left"].value(); }

vector<string> Node::attsplit(string key, string delims) {
	return split( atts[key], delims);
}

Node* Node::getNode(string name) {
	if(name == "p") { return parent; }
	if(name == "prev") { return prev; }
	if(name == "next") { return next; }
	if(name == "last") { return subs.back(); }
	return this;
}

kiwi::Variable Node::getVar(std::string key) {
	vector<string> parts = split(key,".");
	if(parts.size() == 1) {
		return vars[key];
	}
	Node* node = getNode(parts[0]);
	return node->vars[ parts[1] ];

}
vector<string> Node::split(string val, string delims) {
	vector<string> result;
	size_t current;
	size_t next = -1;
	do {
		current = next + 1;
		next = val.find_first_of( delims, current );
		result.push_back( val.substr( current, next - current ) );
	}
	while (next != string::npos);
	return result;
}
