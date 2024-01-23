#pragma once

#include <string>
#include <vector>

#include "pkb/relationship_managers/RelationshipManager.h"

// Map stmt number of print to proc name
class PrintVarName : public RelationshipManager<int, std::string> {
};