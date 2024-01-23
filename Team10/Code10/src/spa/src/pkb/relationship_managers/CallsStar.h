#pragma once

#include <string>
#include <vector>

#include "pkb/relationship_managers/RelationshipManager.h"

class CallsStar : public RelationshipManager<std::string, std::vector<std::string>> {
   public:
    CallsStar() {}
};