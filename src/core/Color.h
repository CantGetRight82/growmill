#pragma once
#include <map>
#include <string>

class Color {
	public:
	float r,g,b,a;
	Color(float r=0, float g=0, float b=0, float a=1) : r(r),g(g),b(b),a(a) {
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

		//TODO
			/* result[0] = ((color>>16)+0f) / 255.0f; */
			/* result[1] = (((color>>8) & 0xFF)+0f) / 255.0f; */
			/* result[2] = ((color&0xFF)+0f) / 255.0f; */
		return Color(1,1,0);
	}

};
