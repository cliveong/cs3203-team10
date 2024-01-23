#pragma once

#include <string>
#include <vector>

#include "pkb/relationship_managers/RelationshipManager.h"

// Map stmt number of calls to proc name
class StmtToProcName : public RelationshipManager<int, std::string> {
};