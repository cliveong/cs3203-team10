#include "ASTNode.h"

#include <memory>

#include "sp/Visitors/Visitor.h"

ASTNode::ASTNode() : type(""), value(""), stmtNum(-1) {}

ASTNode::ASTNode(const std::string& type,
                 const std::string& value, int stmtNum)
    : type(type), value(value), stmtNum(stmtNum) {}

void ASTNode::accept(Visitor& visitor, std::vector<ASTNode>& varNodes) {
    visitor.visitASTNode(shared_from_this(), varNodes);
}

void ASTNode::addChild(std::shared_ptr<ASTNode> child) {
    children.push_back(child);
}

void ASTNode::setAdditionalNodes(std::vector<ASTNode> nodes) {
    additionalNodes = nodes;
}

std::string ASTNode::getType() const {
    return type;
}

std::string ASTNode::getValue() const {
    return value;
}

void ASTNode::setValue(const std::string newVal) {
    this->value = newVal;
}

int ASTNode::getStmtNum() const {
    return stmtNum;
}

std::vector<std::shared_ptr<ASTNode>> ASTNode::getChildren() const {
    return children;
}

// removed child equator
bool ASTNode::operator==(const ASTNode& other) const {
    if (type != other.type || value != other.value || stmtNum != other.stmtNum) {
        return false;
    }
    if (children.size() != other.children.size()) {
        return false;
    }
    for (size_t i = 0; i < children.size(); ++i) {
        if (!(children[i] == other.children[i])) {
            return false;
        }
    }
    return true;
}

bool ASTNode::operator!=(const ASTNode& other) const {
    return !(*this == other);
}

bool ASTNode::operator<(const ASTNode& other) const {
    return stmtNum < other.stmtNum;
}

// modified for subtree matching
// ai-gen start (chatgpt, 0)
bool ASTNode::equals(const ASTNode& other) const {
    if (type != other.type || value != other.value || children.size() != other.children.size() || stmtNum != other.stmtNum) {
        return false;
    }
    for (std::size_t i = 0; i < children.size(); ++i) {
        if (!children[i]->equals(*other.children[i])) {
            return false;
        }
    }
    return true;
}
// ai-gen end
bool ASTNode::matches(const ASTNode& other) const {
    if (type != other.type || value != other.value || children.size() != other.children.size()) {
        return false;
    }
    for (std::size_t i = 0; i < children.size(); ++i) {
        if (!children[i]->matches(*other.children[i])) {
            return false;
        }
    }
    return true;
}
