#pragma once

#include <string>
#include <vector>

#include "pkb/relationship_managers/RelationshipManager.h"

class ModifiesProc : public RelationshipManager<std::string, std::vector<std::string>> {
   public:
    ModifiesProc() {}
};