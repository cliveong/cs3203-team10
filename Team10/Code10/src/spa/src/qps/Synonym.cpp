#include "Synonym.h"

#include <utility>

Synonym::Synonym(SynonymType synonymType, std::string synonymName) : synonymType(synonymType), synonymName(std::move(synonymName)) {}

bool Synonym::operator==(const Synonym& synonym) {
    return this->synonymType == synonym.synonymType && this->synonymName == synonym.synonymName;
}

SynonymType Synonym::getSynonymType() {
    return this->synonymType;
}

std::string Synonym::getSynonymName() {
    return this->synonymName;
}

bool Synonym::isStatement() {
    if (this->synonymType != VARIABLE && this->synonymType != CONSTANT && this->synonymType != PROCEDURE) {
        return true;
    } else {
        return false;
    }
}

bool Synonym::isSynonym() {
    return true;
}

bool Synonym::isAttrRef() {
    return false;
}