#pragma once

#include <stdexcept>

class SyntaxException : public std::runtime_error {
   public:
    explicit SyntaxException(const std::string &errorMsg);
};
