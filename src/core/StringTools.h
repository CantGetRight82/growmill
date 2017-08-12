#pragma once

#include <string>
#include <vector>

class StringTools {
    public:
        static std::vector<std::string> split(std::string val, std::string delims);
        static std::string trim(std::string val);
};


