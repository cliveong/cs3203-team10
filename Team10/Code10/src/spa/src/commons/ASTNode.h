#pragma once

#include <functional>
#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>

#include "Constants.h"

class ModifiesVisitor;
class Visitor;

class ASTNode : public std::enable_shared_from_this<ASTNode> {
   public:
    ASTNode();
    ASTNode(const std::string& type, const std::string& value = "", int stmtNum = -1);

    virtual void accept(Visitor& visitor, std::vector<ASTNode>& varNodes);
    std::vector<ASTNode> additionalNodes;
    void addChild(std::shared_ptr<ASTNode> child);
    void setAdditionalNodes(std::vector<ASTNode> nodes);
    std::string getType() const;
    std::string getValue() const;
    void setValue(const std::string newVal);
    int getStmtNum() const;
    std::vector<std::shared_ptr<ASTNode>> getChildren() const;
    bool operator==(const ASTNode& other) const;
    bool operator!=(const ASTNode& other) const;
    bool operator<(const ASTNode& other) const;
    bool equals(const ASTNode& other) const;
    bool matches(const ASTNode& other) const;

    // ai=gen start (chatgpt, 0)
    struct Hash {
        std::size_t operator()(const ASTNode& node) const {
            std::size_t seed = 0;
            seed ^= std::hash<std::string>()(node.getType()) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
            seed ^= std::hash<std::string>()(node.getValue()) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
            seed ^= std::hash<int>()(node.getStmtNum()) + 0x9e3779b9 + (seed << 6) + (seed >> 2);

            return seed;
        }
    };
    // ai-gen end

   private:
    std::string type;
    std::string value;
    int stmtNum;
    std::vector<std::shared_ptr<ASTNode>> children;
};
