#include <vector>
#include <string>
#ifndef TUPLA_H
#define TUPLA_H

struct Tupla {
    std::vector<std::string> cols;
    std::string to_string() const {
        std::ostringstream oss;
        for (size_t i = 0; i < cols.size(); ++i) {
            if (i > 0) oss << ",";
            oss << cols[i];
        }
        return oss.str();
    }
};

#endif