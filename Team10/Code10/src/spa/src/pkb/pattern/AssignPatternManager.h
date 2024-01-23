#pragma once
#include <algorithm>
#include <iterator>
#include <memory>
#include <string>

#include "../../commons/ASTNode.h"
#include "pkb/pattern/PatternManager.h"

class AssignPatternManager : public PatternManager<std::string, ASTNode> {
};