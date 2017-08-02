#pragma once
#include <iostream>
#include <vector>
#include <map>
#include <math.h>
#include <assert.h>

#include "VarProvider.h"
#include "Color.h"
#include "Rect.h"
#include "../../vendor/kiwi/kiwi/kiwi.h"
#include "AbstractTextSizer.h"

class Node : VarProvider {
	public:
		static Node* zero;
		Node* parent;
		Node* prev;
		Node* next;
    
        std::string tag;

		std::map<std::string,std::string> atts;
		std::vector<Node*> subs;

		std::map<std::string, kiwi::Variable> vars;
        std::vector<std::string> usedVars;
    
		void renderJSON(std::ostream& to);
        double number(std::string key);
        double nodeNumber(std::string val);
		Color color(std::string key);
    
        std::string str(std::string key) {
            return atts[key];
        }

        bool has(std::string key);

		Node() : parent(NULL), prev(NULL), next(NULL) {
			initVars();
		}

        void add(Node* child);
        void clear();
    
		void initVars();
		std::string ids();
		int idx(Node* child);

        Rect rect();

        double top();
        double right();
        double bottom();
        double left();

		Node* getNode(std::string name);
		kiwi::Variable getVar(std::string key);
		void constrain(kiwi::Solver* solver);
        void fillBlanks(kiwi::Solver* solver, AbstractTextSizer& textSizer);
		void addStay(kiwi::Solver* solver, std::string key, float val);
    Node* clone();
    
        std::string getOut(std::string key, std::string val, bool side);
        bool addOutZero(kiwi::Solver* solver, std::string key);
};
