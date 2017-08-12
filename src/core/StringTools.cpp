#include "StringTools.h"

using std::string;
using std::vector;

vector<string> StringTools::split(string val, string delims) {
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

string StringTools::trim(string str) {
    size_t first = str.find_first_not_of(' ');
    if (first == string::npos)
        return "";
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last-first+1));
}
