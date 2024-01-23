#include "SPA.h"

#include <filesystem>
#include <fstream>
#include <iostream>

SPA::SPA() : tokenizer("") {
    queryManager = std::make_shared<QueryManager>(make_shared<ReadFacade>(readFacade));
}

void SPA::loadSourceCode(const std::string& filename) {
    std::ifstream inputFile(filename);

    if (!inputFile.is_open()) {
        throw std::runtime_error("Error: Can't open file");
    }

    std::string code;
    std::string line;

    while (std::getline(inputFile, line)) {
        code += line + "\n";
    }

    inputFile.close();

    sourceCode = code;
}

void SPA::loadStringCode(const std::string& code) { sourceCode = code; }

void SPA::SPAparse() {
    Tokenizer tokenizer(sourceCode);
    std::vector<Token> tokens = tokenizer.tokenize();
    Parser parser(tokens);

    rootNode = parser.parse();

    DesignExtractor designExtractor(writeFacade, rootNode);
    designExtractor.extract();
}

std::vector<std::string> SPA::parseQuery(std::string query) {
    return queryManager->process(query);
}

std::shared_ptr<ReadFacade> SPA::getReadFacade() {
    return make_shared<ReadFacade>(this->readFacade);
}

std::shared_ptr<Storage> SPA::getStorage() { return storage; }
