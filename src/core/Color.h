#pragma once
#include <map>
#include <string>

class Color {
	public:
	float r,g,b,a;
	Color(float r=0, float g=0, float b=0, float a=1) : r(r),g(g),b(b),a(a) {
	}
    
    std::string str() {
        return std::string("rgb(") +
        std::to_string((int)(r*0xFF)) + "," +
        std::to_string((int)(g*0xFF)) + "," +
        std::to_string((int)(b*0xFF)) + ")";
    }
    static float normal(unsigned char big, unsigned char small) {
		return ((big<<4) + small)/255.0;
	}
    
    static unsigned char parseChar(unsigned char c) {
        unsigned char result = c-48;
        if(result > 9){
            result -= 39;
        }
        return result;
    }
	static Color fromString(std::string str) {
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
		return Color( normal(r1,r2), normal(g1,g2), normal(b1,b2) );
	}

};
