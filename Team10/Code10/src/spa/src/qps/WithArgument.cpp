#include "WithArgument.h"

#include <utility>

WithArgument::WithArgument(ArgType type, std::string value, std::shared_ptr<Attribute> attribute) : Argument(type, std::move(value)), attribute(std::move(attribute)) {}

WithArgument::WithArgument(ArgType type, std::shared_ptr<Synonym> synonym, std::shared_ptr<Attribute> attribute) : Argument(type, std::move(synonym)), attribute(std::move(attribute)) {}

std::shared_ptr<Attribute> WithArgument::getAttribute() {
    return attribute;
}
