#pragma once

#include <string>
#include "Elem.h"

enum SynonymType {
    STMT,
    READ,
    PRINT,
    CALL,
    WHILE,
    IF,
    ASSIGN,
    VARIABLE,
    CONSTANT,
    PROCEDURE,
    UNDECLARED
};

class Synonym : public Elem {
   private:
    SynonymType synonymType;
    std::string synonymName;

   public:
    Synonym(SynonymType synonymType, std::string synonymName);
    bool operator==(const Synonym& synonym);
    SynonymType getSynonymType();
    std::string getSynonymName();
    bool isStatement();
    bool isSynonym() override;
    bool isAttrRef() override;
};
