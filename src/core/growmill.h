#pragma once

#include "Node.h"
#include "AbstractVirtualDisk.h"
#include "parse.h"

class GrowMill {
	public:
		static Node* parse(AbstractVirtualDisk& disk, string infile, int outerWidth, int outerHeight) {
			Node* root = NodeParser::parse(disk, infile);
			root->atts["left"] = "0";
			root->atts["top"] = "0";

			if(outerWidth>0) {
				root->atts["width"] = std::to_string(outerWidth);
				root->atts["height"] = std::to_string(outerHeight);
			} else {
				/* root->atts["right"] = "last.right + 0"; */
				/* root->atts["bottom"] = "last.bottom + 0"; */
			}

			/* root->renderJSON(cout); */

			try {
				kiwi::Solver* solver = new kiwi::Solver();
				root->constrain( solver ); 
				solver->updateVariables();
			} catch(kiwi::UnsatisfiableConstraint e) {
				cout<<e.what()<<endl;
				cout<<"--------------------------------------"<<endl;
				kiwi::debug::dump( e.constraint() );
				cout<<"--------------------------------------"<<endl;
			} catch(const char* e) {
				cout<<e<<endl;
			}



			return root;

		}
};
