#pragma once
#include <memory>
#include <string>

#include "Synonym.h"

enum ArgType {
    SYNONYM = 1,
    INT_ARG = 2,
    WILDCARD = 3,
    STRING_ARG = 4,
    EXPRESSION = 5,
    UNKNOWN_ARG = 6
};

class Argument {
    // type:
    // synonym - stmt, procedure, variable, assignment, read, call, while, if, print
    // statementNo - integer
    // wildcard - wildcard
    // "..." - string
    // pattern
   protected:
    ArgType type;

    std::shared_ptr<Synonym> synonym;
    std::string value;

   public:
    Argument(ArgType type, std::shared_ptr<Synonym> synonym);
    Argument(ArgType type, std::string value);
    virtual ~Argument() {}
    bool isSynonym();
    ArgType getType();
    std::shared_ptr<Synonym> getSynonym();
    std::string getValue();
    void setArgType(ArgType argType);
    void setSynonym(std::shared_ptr<Synonym> synonym);
    void setValue(std::string value);
    bool operator==(Argument arg);
};