#include "SyntaxException.h"

SyntaxException::SyntaxException(const std::string& errorMsg) : runtime_error(errorMsg) {}
