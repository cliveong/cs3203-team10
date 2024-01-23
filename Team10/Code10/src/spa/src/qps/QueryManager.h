#pragma once

#include <memory>
#include <string>
#include <vector>

#include "pkb/ReadFacade.h"

class QueryManager {
    std::shared_ptr<ReadFacade> readFacade;

   public:
    QueryManager() = default;
    QueryManager(std::shared_ptr<ReadFacade> readFacade);
    std::vector<std::string> process(const std::string& query);
};
