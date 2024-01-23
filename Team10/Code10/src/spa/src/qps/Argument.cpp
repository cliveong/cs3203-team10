#include "Argument.h"

#include <utility>

Argument::Argument(ArgType type, std::shared_ptr<Synonym> synonym) : type(type), synonym(std::move(synonym)){};
Argument::Argument(ArgType type, std::string value) : type(type), value(std::move(value)){};

ArgType Argument::getType() {
    return type;
}

std::shared_ptr<Synonym> Argument::getSynonym() {
    return synonym;
}

std::string Argument::getValue() {
    return value;
}

void Argument::setArgType(ArgType argType) {
    this->type = argType;
}

void Argument::setSynonym(std::shared_ptr<Synonym> newSynonym) {
    this->synonym = std::move(newSynonym);
}

void Argument::setValue(std::string newValue) {
    this->value = std::move(newValue);
}

bool Argument::operator==(Argument arg) {
    if (isSynonym()) {
        return type == arg.type && (*synonym) == (*arg.synonym);
    } else {
        return type == arg.type && value == arg.value;
    }
}

bool Argument::isSynonym() {
    return this->type == SYNONYM;
}
