#pragma once

#include <string>
#include <vector>

#include "pkb/relationship_managers/RelationshipManager.h"

// Map stmt number of read to proc name
class ReadVarName : public RelationshipManager<int, std::string> {
};