#include "Attribute.h"
#include <string>

Attribute::Attribute(AttrName attrName) : attrName(attrName) {
    attrType = attrTypeMap[attrName];
}

Attribute::Attribute(const std::string& attrName) {
    this->attrName = attrNameMap[attrName];
    this->attrType = attrTypeMap[this->attrName];
}

AttrName Attribute::getAttrName() {
    return this->attrName;
};
AttrType Attribute::getAttrType() {
    return this->attrType;
}

bool Attribute::operator==(const Attribute& attribute) {
    return this->attrName == attribute.attrName;
};