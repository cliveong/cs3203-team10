#pragma once

#include <map>
#include <string>

enum AttrType {
    NAME_ATTRTYPE,
    INTEGER_ATTRTYPE
};

enum AttrName {
    PROCNAME_ATTR,
    VARNAME_ATTR,
    VALUE_ATTR,
    STMT_ATTR
};

class Attribute {
   private:
    AttrName attrName;
    AttrType attrType;
    std::map<AttrName, AttrType> attrTypeMap = {
        {PROCNAME_ATTR, NAME_ATTRTYPE},
        {VARNAME_ATTR, NAME_ATTRTYPE},
        {VALUE_ATTR, INTEGER_ATTRTYPE},
        {STMT_ATTR, INTEGER_ATTRTYPE}};

    std::map<std::string, AttrName> attrNameMap = {
        {"procName", PROCNAME_ATTR},
        {"varName", VARNAME_ATTR},
        {"value", VALUE_ATTR},
        {"stmt", STMT_ATTR}};

   public:
    explicit Attribute(const std::string& attrName);
    explicit Attribute(AttrName attrName);
    AttrName getAttrName();
    AttrType getAttrType();
    bool operator==(const Attribute& attribute);
};
