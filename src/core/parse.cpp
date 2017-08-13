#include "parse.h"
#include "SymbolPos.h"

#include "StringTools.h"

#include "../../vendor/kiwi/kiwi/kiwi.h"
#include "../../vendor/kiwi/kiwi/debug.h"

using std::string;
using std::vector;

string trimBefore( const char* buffer, std::vector<SymbolPos>& positions, int i) {
	int end = positions[i].pos-1;
	int start = i>0 ? positions[i-1].pos+1 : 0;
	while( start < end && isspace(buffer[start]) ) { start++; }
	while( end >= start && isspace(buffer[end]) ) { end--; }
	if(end>=start) {
		size_t size = end-start+1;
		return string( buffer+start, size);
	}
	return "";

}


bool trbl(Node* node, string key, string val) {
	int size = key.size();
	if(size>4) { return false; }
	for(int i=0; i<size; i++) {
		if(strchr("trbl", key[i]) == NULL) {
			return false;
		}
	}

	for(int i=0; i<size; i++) {
		switch(key[i]) {
			case 't': node->atts["top"] = val; break;
			case 'r': node->atts["right"] = val; break;
			case 'b': node->atts["bottom"] = val; break;
			case 'l': node->atts["left"] = val; break;
		}
	}
	return true;
}


Node* NodeParser::parse(AbstractVirtualDisk& disk, string infile) {

	string dir = infile.substr(0, infile.find_last_of('/')+1);

	string str = disk.getContents(infile);
	size_t pos;
	while(1) {
		pos = str.find("include(");
		if(pos == string::npos) { break; }

		size_t end = str.find(";", pos);
		assert( end != string::npos );

		size_t filepos = pos + 9;
		string file = str.substr( filepos, end-filepos-2);

		string include = disk.getContents( dir + file );
		str = str.substr(0, pos) + include + str.substr(end+1);
	}

	const char* buffer = str.c_str();
	vector<SymbolPos> positions = SymbolPos::parse( str, "{}:;`" );

	int level = 0;
	bool inString = false;
	for(SymbolPos sp : positions) {
		char c = sp.symbol;
		if(c == '`') { inString = !inString; }
		else if(c == '{' && !inString) { level++; }
		else if(c == '}' && !inString) { level--; }

	}

	if(level != 0) {
        throw std::runtime_error("{ and } characters do not match up");
	}



	vector<Node*> stack;
	Node* root;
	Node* top;
	root = top = new Node();
	stack.push_back(root);
	string key;
	for(int i=0; i<positions.size(); i++) {
		char symbol = positions[i].symbol;

		string str = trimBefore(buffer, positions, i);
		if(symbol == '{') {
            Node* node = NULL;
			Node* clone = NULL;
            
            string tag("");
            string name("");
            
            if(str.size()) {
                vector<string> parts = StringTools::split(str," ");
                for(string part : parts) {
                    if(part[0] == '<') {
                        if(part.size() == 1) {
                            clone = top->subs.back();
                        } else {
                            clone = root->find( part.substr(1) );
                        }
                    } else if(part[0] == '#') {
                        name = part.substr(1);
                        node = top->find(name);
                        
                    } else {
                        tag = part;
                    }
                }
            }

            if(!node) {
                if(clone) {
                    node = clone->clone();
                } else {
                    node = new Node();
                }
                node->tag = tag;
                node->name = name;

                if(top) {
                    top->add( node );
                }
               
            }
            stack.push_back( node );
			top = node;
		} else if(symbol == '}') {
			stack.pop_back();
			top = stack.back();
		} else if(symbol == ':') {
			key = str;
		} else if(symbol == ';') {
			if(key.size() && str.size()) {
				if(trbl(top, key, str)) {
				} else if(key.compare("size") == 0) {
                    vector<string> parts = StringTools::split(str," ");
					if(parts.size() == 2) {
						top->atts[ "width" ] = parts[0];
						top->atts[ "height" ] = parts[1];
					} else {
						top->atts[ "width" ] = str;
						top->atts[ "height" ] = str;
					}
				} else {
					top->atts[ key ] = str;
				}
                key = "";
            } else {
                if(top->has(str)) {
                    top->atts.erase(str);
                } else if(str == "clear") {
                    top->clear();
                }
            }
		}
	}
	return root;
}
