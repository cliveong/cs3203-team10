#pragma once

#include <string>
#include <vector>

#include "pkb/relationship_managers/RelationshipManager.h"

class UsesS : public RelationshipManager<int, std::vector<std::string>> {
   public:
    UsesS() {}
};