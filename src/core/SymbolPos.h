#pragma once

class SymbolPos {
	public:
	static std::vector<SymbolPos> parse( std::string str, const char* delimeters ) {
		const char* buffer = str.c_str();
		size_t size = str.size();

		std::vector<SymbolPos> result;
		for(int i=0; i<size; i++) {
			if(strchr(delimeters, buffer[i]) != NULL) {
				char c = buffer[i];
				result.push_back( SymbolPos( c, i ) );
			}
		}
		return result;
	}

	char symbol;
	size_t pos;
	SymbolPos( char symbol, size_t pos ) : symbol(symbol), pos(pos) {
	}
};
