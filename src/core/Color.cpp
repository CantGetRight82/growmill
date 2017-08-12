#include "Color.h"

using std::string;
    
std::string Color::str() {
	return std::string("rgba(") +
		std::to_string((int)(r*0xFF)) + "," +
		std::to_string((int)(g*0xFF)) + "," +
		std::to_string((int)(b*0xFF)) + "," + std::to_string(a) + ")";
}

float Color::normal(unsigned char big, unsigned char small) {
	return ((big<<4) + small)/255.0;
}
    
unsigned char Color::parseChar(unsigned char c) {
    if(c < 58) { return c-48; }
    if(c < 97) { return c-55; }
    return c - 87;
}

Color Color::fromString(std::string str) {
	std::map<std::string,Color> predefined = {
		{ "black", Color(0,0,0) },
		{ "gray", Color(0.5,0.5,0.5) },
		{ "white", Color(1,1,1) },
		{ "red", Color(1,0,0) },
		{ "green", Color(0,1,0) },
		{ "blue", Color(0,0,1) }
	};

	if(predefined.count(str)) {
		return predefined[str];
	}
	
    string color;
    double alpha;
    
    size_t pos = str.find('.');
    if(pos !=std::string::npos) {
        alpha = std::stod( str.substr(pos), 0);
        str = str.substr(0,pos);
    } else {
        alpha = 1;
    }
	
	unsigned char r1,r2,g1,g2,b1,b2;
	if(str.size() == 4) {
		r1 = r2 = parseChar(str[1]);
		g1 = g2 = parseChar(str[2]);
		b1 = b2 = parseChar(str[3]);
	} else {
		r1 = parseChar(str[1]);
		r2 = parseChar(str[2]);
		g1 = parseChar(str[3]);
		g2 = parseChar(str[4]);
		b1 = parseChar(str[5]);
		b2 = parseChar(str[6]);
	}
	return Color( normal(r1,r2), normal(g1,g2), normal(b1,b2), alpha );
}

