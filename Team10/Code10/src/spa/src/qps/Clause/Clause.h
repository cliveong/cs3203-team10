#pragma once

#include <memory>
#include <string>

#include "../Argument.h"

class Clause {
   protected:
    std::shared_ptr<Argument> arg1;
    std::shared_ptr<Argument> arg2;
    std::string name;
    bool notOpr;

   public:
    Clause() = default;
    Clause(std::shared_ptr<Argument> arg1, std::shared_ptr<Argument> arg2, std::string name, bool notOpr);

    virtual ~Clause() {}

    void setArguments(std::shared_ptr<Argument> arg1, std::shared_ptr<Argument> arg2);
    std::shared_ptr<Argument> getFirstArg();
    std::shared_ptr<Argument> getSecondArg();
    std::string getName();
    bool isNot();
    virtual bool operator==(const Clause& clause);
};