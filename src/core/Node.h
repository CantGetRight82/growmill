#pragma once
#include <iostream>
#include <vector>
#include <map>
#include <math.h>
#include <assert.h>

#include "VarProvider.h"
#include "../../vendor/kiwi/kiwi/kiwi.h"

using namespace std;

class Node : VarProvider {
	public:
		static Node* zero;
		Node* parent;
		Node* prev;
		Node* next;

		map<string,string> atts;
		vector<Node*> subs;

		map<string, kiwi::Variable> vars;

		void renderJSON(std::ostream& to);
		int atti(string key);
		float attf(string key);
		uint32_t color(string key);

		Node() : parent(NULL), prev(NULL), next(NULL) {
			initVars();
		}

		void initVars();
		string ids();
		int idx(Node* child);

		float top();
		float right();
		float bottom();
		float left();

		vector<string> attsplit(string key, string delims);
		Node* getNode(string name);
		kiwi::Variable getVar(std::string key);
		vector<string> split(string val, string delims);
		void constrain(kiwi::Solver* solver);
		void addStay(kiwi::Solver* solver, string key, float val);
		Node* clone();
};
