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
