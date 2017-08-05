#include "Node.h"

#include <math.h>
#include "ExpressionParser.h"
#include "StringTools.h"

using namespace kiwi;
using std::string;
using std::vector;
using std::map;

using std::cout;
using std::endl;


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
                vector<string> parts = StringTools::split(val," ");
				assert(parts.size() == 2);
                
                val = getOut( key, parts[1], side);

			}
			ExpressionParser::parse( key, val, *solver, *this);
		}
	}

    for( Node* n : subs) {
        n->constrain( solver );
    }

}


std::string Node::getOut(std::string key, std::string val, bool side) {
    if(!side) {
        if(key == "top") {
            return prev ? "prev.bottom + " + val : "p.top + " + val;
        }
        if(key == "left") {
            return "p.left + " + val;
        }
        if(key == "right") {
            return "p.right - " + val;
        }
        if(key == "bottom") {
            return next ? "next.top - "+val : "p.bottom - "+val;
        }
    } else {
        if(key == "top") {
            return "p.top + " + val;
        }
        if(key == "left") {
            return prev ? "prev.right + "+val : "p.left + " + val;
        }
        if(key == "right") {
            return next ? "next.left - "+val : "p.right - " + val;
        }
        if(key == "bottom") {
            return "p.bottom - "+val;
        }
    }
    return "";
}

bool Node::addOutZero(kiwi::Solver* solver, std::string key) {
    if(std::find(usedVars.begin(), usedVars.end(), key) == usedVars.end()) {
//        ExpressionParser::parse( key, getOut(key, "0", false), *solver, *this);
        ExpressionParser::parse( key, string("p."+key), *solver, *this);
        return true;
    }
    return false;
}
void Node::fillBlanks(Solver* solver, AbstractTextSizer& textSizer) {
    
    int xcount = 0, ycount = 0;
    for(string varkey : usedVars) {
        if(varkey == "left" || varkey == "width" || varkey == "right") { xcount++; }
        if(varkey == "top" || varkey == "height" || varkey == "bottom") { ycount++; }
    }
    
    if(xcount == 0) { addOutZero(solver, "left"); xcount++; }
    if(ycount == 0) { addOutZero(solver, "top"); ycount++; }
    
    if(has("text")) {
        bool needsWidth = vars["width"].value() <= 0;
        bool needsHeight = vars["height"].value() <= 0;
        if(needsWidth || needsHeight) {
            
            float maxw = needsWidth ? -1 : right()-left();
        
            float outw, outh;
            textSizer.measure(this, maxw, &outw, &outh);
            
            if(needsWidth) {
                addStay(solver, "width", outw);
            }
            
            if(needsHeight) {
                addStay(solver, "height", outh);
            }
        }
    } else {
        if(xcount==1) {
            addOutZero(solver,"left") || addOutZero(solver,"right");
        }
        if(ycount==1) {
            addOutZero(solver,"top") || addOutZero(solver,"bottom");
        }
    }
    
    for( Node* n : subs) {
        n->fillBlanks(solver, textSizer);
    }
}

void Node::addStay(Solver* solver, string key, float val) {
	solver->addEditVariable( vars[key], strength::strong );
	solver->suggestValue( vars[key], val );
}



Color Node::color(string key) {
	string val = atts[key];
	return Color::fromString( val );
}

double Node::nodeNumber(string val) {
    vector<string> parts = StringTools::split(val,".");
    Node* node = parts.size() > 1 ? getNode(parts[0]) : this;
    string key = parts.back();
    if(node->atts.count(key)) {
        return node->number(key);
    }
    if(node->vars.count(key)) {
        return node->vars[ key ].value();
    }
    return 0;

}

double Node::number(string key) {
    if(atts.count(key)) {
        string val = atts[key];
        bool numeric= true;
        for(int i=0; i<val.size(); i++) {
            if(isalpha(val[i])) {
                numeric = false;
                break;
            }
        }
        if(numeric) {
            return strtod( val.c_str(), NULL );
        } else {
            vector<ExpressionPart> parts = ExpressionPart::parse( val );
            double result = 0;
            
            for(ExpressionPart part : parts) {
                double nr = strtod(part.nr.c_str(),NULL);
                if(part.var.size()) {
                    result += nodeNumber( part.var ) * nr * (part.negate?-1:1);
                } else {
                    result += nr * (part.negate ? -1 : 1);
                }
            }
            
            return result;
        }
    }
    return NAN;
}
Node* Node::clone() {
	Node* result = new Node();
	result->atts = this->atts;

    for(Node* sub : subs) {
        result->add( sub->clone() );
    }
	return result;
}


void Node::add(Node* child) {
    child->parent = this;
    if(subs.size()) {
        child->prev = subs.back();
        child->prev->next = child;
    }
    subs.push_back(child);
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
double Node::top() { return vars["top"].value(); }
double Node::right() { return vars["right"].value(); }
double Node::bottom() { return vars["bottom"].value(); }
double Node::left() { return vars["left"].value(); }

int Node::idx(Node* child) {
	for(int i=0; i<subs.size(); i++) {
		if(child == subs[i]) { return i; }
	}
	return -1;
}

Rect Node::rect() {
	float top = vars["top"].value();
	float left = vars["left"].value();
	float bottom = vars["bottom"].value();
	float right = vars["right"].value();

	return Rect( left, top, right-left, bottom-top );
}


Node* Node::getNode(string name) {
	if(name == "p") { return parent; }
	if(name == "prev") { return prev; }
	if(name == "next") { return next; }
	if(name == "last") { return subs.back(); }
    if(name == "sib") {
        if(prev) { return prev; }
        if(next) { return next; }
        return parent;
    }
	return this;
}

void Node::clear() {
    for(Node* sub : subs) {
        sub->parent = sub->prev = sub->next = NULL;
    }
    subs.clear();
}

kiwi::Variable Node::getVar(std::string key) {
	vector<string> parts = StringTools::split(key,".");
    Node* node;
    string varkey;
	if(parts.size() == 1) {
        node = this;
        varkey = key;
    } else {
        node = getNode(parts[0]);
        varkey = parts[1];
    }
    node->usedVars.push_back(varkey);
	return node->vars[ varkey ];

}

