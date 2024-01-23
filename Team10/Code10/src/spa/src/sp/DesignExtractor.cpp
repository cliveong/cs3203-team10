#include "DesignExtractor.h"

#include <algorithm>
#include <map>
#include <set>
#include <vector>

#include "sp/Visitors/CFGVisitor.h"
#include "sp/Visitors/CallsStarVisitor.h"
#include "sp/Visitors/CallsVisitor.h"
#include "sp/Visitors/EntitiesVisitor.h"
#include "sp/Visitors/FollowsVisitor.h"
#include "sp/Visitors/ModifiesVisitor.h"
#include "sp/Visitors/ParentsVisitor.h"
#include "sp/Visitors/PatternVisitor.h"
#include "sp/Visitors/UsesVisitor.h"

DesignExtractor::DesignExtractor(WriteFacade& wf,
                                 std::shared_ptr<ASTNode> rootNode)
    : writeFacade(wf), rootNode(rootNode) {
}

void DesignExtractor::extract() {
    extractEntities(rootNode);
    extractFollows(rootNode);
    extractPattern(rootNode);
    extractParents(rootNode);
    extractCalls(rootNode);
    extractCallsStar(rootNode);
    extractUses(rootNode);
    extractModifies(rootNode);
    extractCFG(rootNode);
}

void DesignExtractor::extractCFG(std::shared_ptr<ASTNode> node) {
    std::vector<ASTNode> varNodes;
    CFGVisitor visitor(writeFacade, rootNode);

    rootNode->accept(visitor, varNodes);
}

void DesignExtractor::extractEntities(std::shared_ptr<ASTNode> node) {
    std::vector<ASTNode> varNodes;
    EntitiesVisitor visitor(writeFacade, rootNode);

    rootNode->accept(visitor, varNodes);
}

void DesignExtractor::extractPattern(std::shared_ptr<ASTNode> rootNode) {
    std::vector<ASTNode> varNodes;
    PatternVisitor visitor(writeFacade, rootNode);

    rootNode->accept(visitor, varNodes);
}

void DesignExtractor::extractModifies(std::shared_ptr<ASTNode> rootNode) {
    std::vector<ASTNode> varNodes;
    ModifiesVisitor visitor(writeFacade, rootNode);

    rootNode->accept(visitor, varNodes);
    clearNodes();
}

void DesignExtractor::extractUses(std::shared_ptr<ASTNode> rootNode) {
    std::vector<ASTNode> varNodes;
    UsesVisitor visitor(writeFacade, rootNode);

    rootNode->accept(visitor, varNodes);
    clearNodes();
}

void DesignExtractor::extractFollows(std::shared_ptr<ASTNode> rootNode) {
    std::vector<ASTNode> varNodes;
    FollowsVisitor visitor(writeFacade, rootNode);

    rootNode->accept(visitor, varNodes);
}

void DesignExtractor::extractCalls(std::shared_ptr<ASTNode> rootNode) {
    std::vector<ASTNode> varNodes;
    CallsVisitor visitor(writeFacade, rootNode);

    rootNode->accept(visitor, varNodes);
}

void DesignExtractor::extractCallsStar(std::shared_ptr<ASTNode> rootNode) {
    std::vector<ASTNode> varNodes;
    CallsStarVisitor visitor(writeFacade, rootNode);

    rootNode->accept(visitor, varNodes);
}

void DesignExtractor::extractParents(std::shared_ptr<ASTNode> rootNode) {
    std::vector<ASTNode> varNodes;
    ParentsVisitor visitor(writeFacade, rootNode);

    rootNode->accept(visitor, varNodes);
}

void DesignExtractor::clearNodes() {
    for (const auto& child : rootNode->getChildren()) {
        child->additionalNodes.clear();
    }
}