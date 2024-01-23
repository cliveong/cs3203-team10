#pragma once

#include "../../commons/CFGNode.h"
#include "Visitor.h"

class CFGVisitor : public Visitor {
   public:
    CFGVisitor(WriteFacade& writeFacade, std::shared_ptr<ASTNode> rootNode)
        : Visitor(writeFacade, rootNode){};

    std::shared_ptr<CFGNode> startNode = std::make_shared<CFGNode>();
    std::shared_ptr<CFGNode> currNode = startNode;

    void visitProgramNode(std::shared_ptr<ASTNode> node,
                          std::vector<ASTNode>& varNodes) override {
        startNode = std::make_shared<CFGNode>();
        currNode = startNode;

        for (const auto& child : node->getChildren()) {
            child->accept(*this, varNodes);
            std::unordered_set<int> visited;
            storeCFG(startNode, visited);
            startNode = std::make_shared<CFGNode>();
            currNode = startNode;
        }
    };

    void storeCFG(std::shared_ptr<CFGNode> cfg,
                  std::unordered_set<int>& visited) {
        if (cfg && cfg->getStmtNum() != -1) {
            if (visited.count(cfg->getStmtNum())) {
                return;
            }
            writeFacade.storeNext(*cfg);
            visited.insert(cfg->getStmtNum());
        }

        for (const auto& successor : cfg->getSuccessors()) {
            storeCFG(successor, visited);
        }
    }

    void updateNodes(std::shared_ptr<CFGNode> cfgNode) {
        if (currNode->getStmtNum() == -1 && currNode->hasParents()) {
            for (const auto& parent : currNode->getParents()) {
                parent->updateSuccessor(cfgNode);
                if (cfgNode->getStmtNum() == -1) {
                    cfgNode->addParents(parent);
                }
            }
        } else {
            currNode->addSuccessor(cfgNode);
        }
    }

    void visitContainerNode(std::shared_ptr<ASTNode> node,
                            std::vector<ASTNode>& varNodes) override {
        std::shared_ptr<CFGNode> cfgNode =
            std::make_shared<CFGNode>(node->getStmtNum());
        updateNodes(cfgNode);

        std::vector<std::shared_ptr<ASTNode>> children = node->getChildren();
        std::shared_ptr<CFGNode> dummyNode = std::make_shared<CFGNode>();

        for (int i = 1; i < children.size(); i++) {
            currNode = cfgNode;
            children[i]->accept(*this, varNodes);
            if (node->getType() == Constants::WHILE_STMT) {
                updateNodes(cfgNode);
                currNode = cfgNode;
            }

            updateNodes(dummyNode);

            if (currNode->getStmtNum() != -1) {
                dummyNode->addParents(currNode);
            }
        }

        currNode = dummyNode;
    };

    void visitNonContainerNode(std::shared_ptr<ASTNode> node) {
        std::shared_ptr<CFGNode> cfgNode =
            std::make_shared<CFGNode>(node->getStmtNum());
        updateNodes(cfgNode);
        currNode = cfgNode;
    }

    void visitCallsNode(std::shared_ptr<ASTNode> node,
                        std::vector<ASTNode>& varNodes) override {
        visitNonContainerNode(node);
    };

    void visitAssignNode(std::shared_ptr<ASTNode> node,
                         std::vector<ASTNode>& varNodes) override {
        visitNonContainerNode(node);
    };

    void visitReadNode(std::shared_ptr<ASTNode> node,
                       std::vector<ASTNode>& varNodes) override {
        visitNonContainerNode(node);
    };

    void visitPrintNode(std::shared_ptr<ASTNode> node,
                        std::vector<ASTNode>& varNodes) override {
        visitNonContainerNode(node);
    };
};