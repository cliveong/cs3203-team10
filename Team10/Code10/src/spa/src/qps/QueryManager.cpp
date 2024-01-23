#include "QueryManager.h"

#include "qps/Evaluator/QueryEvaluator.h"
#include "qps/Parser/QueryParser.h"
#include "qps/Tokenizer/QueryTokenizer.h"
#include "qps/Validator/SemanticValidator.h"
#include "qps/Validator/ValidatorErrors/SemanticError.h"

#include <csetjmp>
#include <csignal>

QueryManager::QueryManager(std::shared_ptr<ReadFacade> readFacade) : readFacade(readFacade){};

static std::jmp_buf jumpBuffer;

void signalHandler(int signal) {
    std::longjmp(jumpBuffer, 1);
}

std::vector<std::string> QueryManager::process(const std::string &query) {
    QueryTokenizer tokenizer(query);

    std::vector<std::shared_ptr<QueryToken>> tokens = tokenizer.tokenize();
    QueryParser parser(tokens);
    std::shared_ptr<Query> queryObject;

    std::signal(SIGSEGV, signalHandler);

    if (setjmp(jumpBuffer) == 0) {
        try {
            queryObject = parser.parse();
        } catch (SyntaxException &syntaxError) {
            return {"SyntaxError"};
        } catch (SemanticError &semanticError) {
            return {"SemanticError"};
        } catch (const std::exception &e) {
            return {"SyntaxError"};
        }

        try {
            SemanticValidator semanticValidator = SemanticValidator();
            semanticValidator.validateSemantic(queryObject);
        } catch (SemanticError &semanticError) {
            return {"SemanticError"};
        }

        QueryEvaluator evaluator = QueryEvaluator(queryObject, readFacade);
        std::vector<std::string> result = evaluator.evaluateQuery();

        // Clear cache from PKB
        readFacade->clearCache();
        return result;
    } else {
        return {"SyntaxError"};
    }
}
