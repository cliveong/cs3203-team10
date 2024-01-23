#pragma once

#include <string>

#include "../../spa/src/qps/QueryManager.h"
#include "./sp/Parsers/Parser.h"
#include "commons/ASTNode.h"
#include "pkb/ReadFacade.h"
#include "pkb/WriteFacade.h"
#include "sp/DesignExtractor.h"
#include "sp/Tokenizer.h"

class SPA {
   public:
    SPA();

    shared_ptr<ASTNode> rootNode;

    void loadSourceCode(const std::string& filename);
    void loadStringCode(const std::string& sourceCode);

    void SPAparse();
    std::vector<std::string> parseQuery(std::string query);

    void printAST(const ASTNode& node);

    std::shared_ptr<ReadFacade> getReadFacade();
    std::shared_ptr<Storage> getStorage();

   private:
    std::string sourceCode;
    Tokenizer tokenizer;
    std::shared_ptr<Storage> storage = std::make_shared<Storage>();
    WriteFacade writeFacade = WriteFacade(storage);
    ReadFacade readFacade = ReadFacade(storage);
    std::shared_ptr<QueryManager> queryManager;
};
