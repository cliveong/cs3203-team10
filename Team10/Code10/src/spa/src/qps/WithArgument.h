#pragma once

#include <map>

#include "../qps/Argument.h"
#include "Attribute.h"

class WithArgument : public Argument {
   private:
    std::shared_ptr<Attribute> attribute;

   public:
    WithArgument(ArgType type, std::string value, std::shared_ptr<Attribute> attribute);
    WithArgument(ArgType type, std::shared_ptr<Synonym> synonym, std::shared_ptr<Attribute> attribute);
    std::shared_ptr<Attribute> getAttribute();
};
