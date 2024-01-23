#include "AttrRef.h"

AttrRef::AttrRef(std::shared_ptr<Synonym> synonym, std::shared_ptr<Attribute> attribute) : synonym(std::move(synonym)), attribute(std::move(attribute)) {}

std::shared_ptr<Synonym> AttrRef::getSynonym() {
    return this->synonym;
}

std::shared_ptr<Attribute> AttrRef::getAttribute() {
    return this->attribute;
}

bool AttrRef::operator==(const AttrRef& attrRef) {
    return *(this->synonym) == *(attrRef.synonym) && *(this->attribute) == *(attrRef.attribute);
}

bool AttrRef::isSynonym() {
    return false;
}

bool AttrRef::isAttrRef() {
    return true;
}
