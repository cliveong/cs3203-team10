#pragma once

#include "Attribute.h"
#include "Elem.h"
#include "Synonym.h"

#include <memory>

class AttrRef : public Elem {
   private:
    std::shared_ptr<Synonym> synonym;
    std::shared_ptr<Attribute> attribute;

   public:
    AttrRef(std::shared_ptr<Synonym> synonym, std::shared_ptr<Attribute> attribute);
    std::shared_ptr<Synonym> getSynonym();
    std::shared_ptr<Attribute> getAttribute();
    bool operator==(const AttrRef& attrRef);
    bool isSynonym() override;
    bool isAttrRef() override;
};
