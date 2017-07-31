#include "ExpressionPart.h"

using std::string;

double ExpressionPart::number() {
	double result = strtod(nr.c_str(), NULL);
	if(negate) {
		result = -result;
	}
    if(divide) {
        result = 1.0/result;
    }
	return result;
}

std::vector<ExpressionPart> ExpressionPart::parse(std::string str) {
	std::vector<ExpressionPart> seq;
    
    int start = 0;
    char first = str[0];
    if(first == '<' || first == '>') {
        seq.push_back( ExpressionPart() );
        seq.back().var = string(first,1);
        start++;
    }
    const char* delimeters = " */-+";
    
    seq.push_back( ExpressionPart() );
    
    for(int i=start; i<str.size(); i++) {
        char c = str[i];
        switch(c) {
            case ' ':
            case '*':
                break;
                
            case '/':
                seq.back().divide = true;
                break;
                
            case '+':
                seq.push_back( ExpressionPart() );
                break;
                
            case '-':
                seq.push_back( ExpressionPart() );
                seq.back().negate = true;
                break;
                
            default:
                size_t end = str.find_first_of(delimeters, i);
                if(end == string::npos) {
                    end = str.size();
                }
                size_t len = end-i;
                string sub = str.substr(i,len);
                if(isdigit(c)) {
                    seq.back().nr = sub;
                } else {
                    seq.back().var = sub;
                }
                i += len-1;
        }
        
    }
    return seq;
};



