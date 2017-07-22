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

class Node : VarProvider {
	public:
		static Node* zero;
		Node* parent;
		Node* prev;
		Node* next;

		std::map<std::string,std::string> atts;
		std::vector<Node*> subs;

		std::map<std::string, kiwi::Variable> vars;

		void renderJSON(std::ostream& to);
		int atti(std::string key);
		float attf(std::string key);
		Color color(std::string key);
    
        std::string str(std::string key) {
            return atts[key];
        }

        bool has(std::string key);

		Node() : parent(NULL), prev(NULL), next(NULL) {
			initVars();
		}

		void initVars();
		std::string ids();
		int idx(Node* child);

		Rect rect() {
			return Rect( left(), top(), right()-left(), bottom()-top() );
		}

		float top();
		float right();
		float bottom();
		float left();

		std::vector<std::string> attsplit(std::string key, std::string delims);
		Node* getNode(std::string name);
		kiwi::Variable getVar(std::string key);
		std::vector<std::string> split(std::string val, std::string delims);
		void constrain(kiwi::Solver* solver);
		void addStay(kiwi::Solver* solver, std::string key, float val);
		Node* clone();
};
