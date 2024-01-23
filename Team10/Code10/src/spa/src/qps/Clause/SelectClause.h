#pragma once

#include <memory>
#include <string>
#include <vector>

#include "qps/Synonym.h"

class SelectClause {
    std::vector<std::shared_ptr<Elem>> elements;
    bool returnBool;

   public:
    explicit SelectClause(std::vector<std::shared_ptr<Elem>> elements, bool returnBool);
    std::vector<std::shared_ptr<Elem>> getElements();
    bool isReturnBool();
    bool operator==(const SelectClause& selectClause);
};