#include "SelectClause.h"
#include "qps/AttrRef.h"

#include <memory>
#include <utility>

SelectClause::SelectClause(std::vector<std::shared_ptr<Elem>> elements, bool returnBool) : elements(std::move(elements)), returnBool(returnBool){};

std::vector<std::shared_ptr<Elem>> SelectClause::getElements() {
    return elements;
}

bool SelectClause::isReturnBool() {
    return returnBool;
}

static bool elemPred(const std::shared_ptr<Elem>& elem1, const std::shared_ptr<Elem>& elem2) {
    auto* synonym1 = dynamic_cast<Synonym*>(&(*elem1));
    auto* attrRef1 = dynamic_cast<AttrRef*>(&(*elem1));

    auto* synonym2 = dynamic_cast<Synonym*>(&(*elem2));
    auto* attrRef2 = dynamic_cast<AttrRef*>(&(*elem2));

    if (synonym1 != nullptr) {      // first elem is synonym
        if (synonym2 == nullptr) {  //second elem not synonym
            return false;
        } else {  // second elem is synonym
            return (*synonym1) == (*synonym2);
        }
    } else {                        // first elem is attrRef
        if (attrRef2 == nullptr) {  // second elem is not attrRef
            return false;
        } else {  // second elem is attrRef
            return (*attrRef1) == (*attrRef2);
        }
    }
}

bool SelectClause::operator==(const SelectClause& selectClause) {
    return returnBool == selectClause.returnBool && std::equal(elements.begin(), elements.end(),
                                                               selectClause.elements.begin(), selectClause.elements.end(),
                                                               elemPred);
};