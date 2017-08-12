#pragma once
#include <iostream>
#include <vector>
#include <map>
#include <set>
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
        std::string name;

		std::map<std::string,std::string> atts;
		std::vector<Node*> subs;

		std::map<std::string, kiwi::Variable> vars;
        std::set<std::string> usedVars;
    
		void renderJSON(std::ostream& to);
        double number(std::string key);
        double nodeNumber(std::string val);
		Color color(std::string key);
    
        std::string str(std::string key) {
            return atts[key];
        }

        Node* has(std::string key, bool parents = false);

		Node() : parent(NULL), prev(NULL), next(NULL) {
			initVars();
		}

        void add(Node* child);
        void clear();
    
		void initVars();
		std::string ids();
		int idx(Node* child);

        Rect rect();
        Rect local();

        double top();
        double right();
        double bottom();
        double left();
    
        Node* find(std::string name);

		Node* getNode(std::string path);
    
		kiwi::Variable getVar(std::string key);
		void constrain(kiwi::Solver* solver);
        void fillBlanks(kiwi::Solver* solver, AbstractTextSizer& textSizer);
		void addStay(kiwi::Solver* solver, std::string key, float val);
    Node* clone();
    
    std::string getIn(std::string key, std::string val, bool side);
    std::string getOut(std::string key, std::string val, bool side);
    void addOutZero(kiwi::Solver* solver, std::string key);
};
