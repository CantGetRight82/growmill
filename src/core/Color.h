#pragma once
#include <map>
#include <string>

class Color {
	public:
	float r,g,b,a;
	Color(float r=0, float g=0, float b=0, float a=1) : r(r),g(g),b(b),a(a) {
	}
    
    std::string str();
    static float normal(unsigned char big, unsigned char small);
    static unsigned char parseChar(unsigned char c);
	static Color fromString(std::string str);

};
