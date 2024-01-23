#pragma once

#include <algorithm>
#include <string>
#include <vector>

class TestUtils {
   public:
    static bool is_same_vector(std::vector<std::string> v1, std::vector<std::string> v2) {
        if (v1.size() != v2.size()) {
            return false;
        }
        return std::is_permutation(v1.begin(), v1.end(), v2.begin());
    }
};
