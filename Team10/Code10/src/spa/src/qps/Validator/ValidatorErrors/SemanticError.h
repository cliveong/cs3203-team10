#pragma once

#include <exception>

class SemanticError : public std::exception {
   public:
    char* what();
};
