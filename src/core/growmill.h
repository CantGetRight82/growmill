#pragma once

#include "Node.h"
#include "AbstractVirtualDisk.h"
#include "AbstractTextSizer.h"
#include "parse.h"



class GrowMill {
	public:
		static Node* parse(
                           AbstractVirtualDisk& disk,
                           AbstractTextSizer& textSizer,
                           std::string infile, int outerWidth, int outerHeight) {
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

			/* root->renderJSON(std::cout); */

			try {
				kiwi::Solver* solver = new kiwi::Solver();
				root->constrain( solver );
                solver->updateVariables();
                root->fillBlanks( solver, textSizer );
                solver->updateVariables();
				
			} catch(kiwi::UnsatisfiableConstraint e) {
				std::cout<<e.what()<<std::endl;
				std::cout<<"--------------------------------------"<<std::endl;
				kiwi::debug::dump( e.constraint() );
				std::cout<<"--------------------------------------"<<std::endl;
			} catch(const char* e) {
				std::cout<<e<<std::endl;
			}



			return root;

		}
};
