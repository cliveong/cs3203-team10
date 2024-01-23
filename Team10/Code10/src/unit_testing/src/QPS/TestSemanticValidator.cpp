#include <memory>
#include <string>
#include <vector>

#include "catch.hpp"
#include "pkb/ReadFacade.h"
#include "pkb/Storage.h"
#include "qps/QueryManager.h"

using namespace std;

TEST_CASE("Test Semantic for Select") {
    std::shared_ptr<Storage> storageManager = std::make_shared<Storage>();
    std::shared_ptr<ReadFacade> readFacade = std::make_shared<ReadFacade>(storageManager);
    QueryManager queryManager = QueryManager(readFacade);

    SECTION("Synonym undeclared for single return") {
        std::string queryString = "variable v, v1;\nSelect a";

        std::vector<std::string> queryResult = queryManager.process(queryString);
        std::vector<std::string> expectedResult = {"SemanticError"};

        REQUIRE(queryResult == expectedResult);
    }

    SECTION("Synonym undeclared for tuple return") {
        std::string queryString = "stmt s; variable v, v1;\nSelect <a, a1, a2>";

        std::vector<std::string> queryResult = queryManager.process(queryString);
        std::vector<std::string> expectedResult = {"SemanticError"};

        REQUIRE(queryResult == expectedResult);
    }

    SECTION("Synonym BOOLEAN undeclared for tuple return") {
        std::string queryString = "stmt s; variable v, v1;\nSelect < BOOLEAN, s>";

        std::vector<std::string> queryResult = queryManager.process(queryString);
        std::vector<std::string> expectedResult = {"SemanticError"};

        REQUIRE(queryResult == expectedResult);
    }
}

TEST_CASE("Test Semantic for Follows") {
    std::shared_ptr<Storage> storageManager = std::make_shared<Storage>();
    std::shared_ptr<ReadFacade> readFacade = std::make_shared<ReadFacade>(storageManager);
    QueryManager queryManager = QueryManager(readFacade);

    SECTION("Test Follows first type variable") {
        std::string queryString = "assign a; variable v, v1;\nSelect a such that Follows(v, a)";

        std::vector<std::string> queryResult = queryManager.process(queryString);
        std::vector<std::string> expectedResult = {"SemanticError"};

        REQUIRE(queryResult == expectedResult);
    }
    SECTION("Test Follows second type variable") {
        std::string queryString = "stmt s; variable v, v1;\nSelect s such that Follows(s, v)";

        std::vector<std::string> queryResult = queryManager.process(queryString);
        std::vector<std::string> expectedResult = {"SemanticError"};

        REQUIRE(queryResult == expectedResult);
    }
    SECTION("Test Follows wrong both arg type") {
        std::string queryString = "assign a; variable v, v1;\nSelect a such that Follows(v, v1)";

        std::vector<std::string> queryResult = queryManager.process(queryString);
        std::vector<std::string> expectedResult = {"SemanticError"};

        REQUIRE(queryResult == expectedResult);
    }
}

TEST_CASE("Test Semantic for Parent") {
    std::shared_ptr<Storage> storageManager = std::make_shared<Storage>();
    std::shared_ptr<ReadFacade> readFacade = std::make_shared<ReadFacade>(storageManager);
    QueryManager queryManager = QueryManager(readFacade);

    SECTION("Test Parent first type variable") {
        std::string queryString = "assign a; variable v, v1;\nSelect a such that Parent(v, a)";

        std::vector<std::string> queryResult = queryManager.process(queryString);
        std::vector<std::string> expectedResult = {"SemanticError"};

        REQUIRE(queryResult == expectedResult);
    }
    SECTION("Test Parent second type variable") {
        std::string queryString = "stmt s; variable v, v1;\nSelect s such that Parent(s, v)";

        std::vector<std::string> queryResult = queryManager.process(queryString);
        std::vector<std::string> expectedResult = {"SemanticError"};

        REQUIRE(queryResult == expectedResult);
    }
    SECTION("Test Parent wrong both arg type") {
        std::string queryString = "assign a; variable v, v1;\nSelect a such that Parent(v, v1)";

        std::vector<std::string> queryResult = queryManager.process(queryString);
        std::vector<std::string> expectedResult = {"SemanticError"};

        REQUIRE(queryResult == expectedResult);
    }
}

TEST_CASE("Test Semantic for Modifies") {
    std::shared_ptr<Storage> storageManager = std::make_shared<Storage>();
    std::shared_ptr<ReadFacade> readFacade = std::make_shared<ReadFacade>(storageManager);
    QueryManager queryManager = QueryManager(readFacade);

    SECTION("Test Modifies first type variable") {
        std::string queryString = "assign a; variable v, v1;\nSelect a such that Modifies(v, v1)";

        std::vector<std::string> queryResult = queryManager.process(queryString);
        std::vector<std::string> expectedResult = {"SemanticError"};

        REQUIRE(queryResult == expectedResult);
    }
    SECTION("Test Modifies first type print") {
        std::string queryString = "print p; variable v, v1;\nSelect p such that Modifies(p, v1)";

        std::vector<std::string> queryResult = queryManager.process(queryString);
        // should not throw semantic error for print statement
        std::vector<std::string> expectedResult;

        REQUIRE(queryResult == expectedResult);
    }
    SECTION("Test Modifies second type stmt") {
        std::string queryString = "stmt s, s1; variable v, v1;\nSelect s such that Modifies(s, s1)";

        std::vector<std::string> queryResult = queryManager.process(queryString);
        std::vector<std::string> expectedResult = {"SemanticError"};

        REQUIRE(queryResult == expectedResult);
    }
    SECTION("Test Modifies wrong both arg type") {
        std::string queryString = "assign a; variable v, v1;\nSelect a such that Modifies(v, a)";

        std::vector<std::string> queryResult = queryManager.process(queryString);
        std::vector<std::string> expectedResult = {"SemanticError"};

        REQUIRE(queryResult == expectedResult);
    }
}

TEST_CASE("Test Semantic for Uses") {
    std::shared_ptr<Storage> storageManager = std::make_shared<Storage>();
    std::shared_ptr<ReadFacade> readFacade = std::make_shared<ReadFacade>(storageManager);
    QueryManager queryManager = QueryManager(readFacade);

    SECTION("Test Uses first type variable") {
        std::string queryString = "assign a; variable v, v1;\nSelect a such that Uses(v, v1)";

        std::vector<std::string> queryResult = queryManager.process(queryString);
        std::vector<std::string> expectedResult = {"SemanticError"};

        REQUIRE(queryResult == expectedResult);
    }
    SECTION("Test Uses first type read") {
        std::string queryString = "read r; variable v, v1;\nSelect r such that Uses(r, v1)";

        std::vector<std::string> queryResult = queryManager.process(queryString);
        // should not throw semantic error for read statement
        std::vector<std::string> expectedResult;

        REQUIRE(queryResult == expectedResult);
    }
    SECTION("Test Uses second type stmt") {
        std::string queryString = "stmt s, s1; variable v, v1;\nSelect s such that Uses(s, s1)";

        std::vector<std::string> queryResult = queryManager.process(queryString);
        std::vector<std::string> expectedResult = {"SemanticError"};

        REQUIRE(queryResult == expectedResult);
    }
    SECTION("Test Uses wrong both arg type") {
        std::string queryString = "assign a; variable v, v1;\nSelect a such that Uses(v, a)";

        std::vector<std::string> queryResult = queryManager.process(queryString);
        std::vector<std::string> expectedResult = {"SemanticError"};

        REQUIRE(queryResult == expectedResult);
    }
}

TEST_CASE("Test Semantic for Pattern") {
    std::shared_ptr<Storage> storageManager = std::make_shared<Storage>();
    std::shared_ptr<ReadFacade> readFacade = std::make_shared<ReadFacade>(storageManager);
    QueryManager queryManager = QueryManager(readFacade);

    SECTION("Test Assign Pattern wrong first arg synonym") {
        std::string queryString = "assign a; stmt s;\nSelect a pattern a(s, _\"a\"_)";

        std::vector<std::string> queryResult = queryManager.process(queryString);
        std::vector<std::string> expectedResult = {"SemanticError"};

        REQUIRE(queryResult == expectedResult);
    }

    SECTION("Test Assign Pattern wrong synonym") {
        std::string queryString = "print pn; stmt s;\nSelect pn pattern pn(_, _\"a\"_)";

        std::vector<std::string> queryResult = queryManager.process(queryString);
        std::vector<std::string> expectedResult = {"SemanticError"};

        REQUIRE(queryResult == expectedResult);
    }

    SECTION("Test If Pattern wrong first arg synonym") {
        std::string queryString = "if if; stmt s;\nSelect s pattern if(s, _, _)";

        std::vector<std::string> queryResult = queryManager.process(queryString);
        std::vector<std::string> expectedResult = {"SemanticError"};

        REQUIRE(queryResult == expectedResult);
    }

    SECTION("Test If Pattern wrong synonym") {
        std::string queryString = "assign a;\nSelect a pattern a(s, _, _)";

        std::vector<std::string> queryResult = queryManager.process(queryString);
        std::vector<std::string> expectedResult = {"SemanticError"};

        REQUIRE(queryResult == expectedResult);
    }

    SECTION("Test While Pattern wrong first arg synonym") {
        std::string queryString = "while w; stmt s;\nSelect w pattern w(s, _)";

        std::vector<std::string> queryResult = queryManager.process(queryString);
        std::vector<std::string> expectedResult = {"SemanticError"};

        REQUIRE(queryResult == expectedResult);
    }

    SECTION("Test While Pattern wrong synonym") {
        std::string queryString = "read re; stmt s;\nSelect re pattern re(s, _)";

        std::vector<std::string> queryResult = queryManager.process(queryString);
        std::vector<std::string> expectedResult = {"SemanticError"};

        REQUIRE(queryResult == expectedResult);
    }
}

TEST_CASE("Test Semantic for With") {
    std::shared_ptr<Storage> storageManager = std::make_shared<Storage>();
    std::shared_ptr<ReadFacade> readFacade = std::make_shared<ReadFacade>(storageManager);
    QueryManager queryManager = QueryManager(readFacade);
    SECTION("Procedure Invalid varName Test") {
        std::string queryString = "procedure p;\nSelect p  with p.varName = \"main\"";

        std::vector<std::string> queryResult = queryManager.process(queryString);
        std::vector<std::string> expectedResult = {"SemanticError"};

        REQUIRE(queryResult == expectedResult);
    }

    SECTION("Procedure Invalid stmt# Test") {
        std::string queryString = "procedure p;\nSelect p  with p.stmt# = 1";

        std::vector<std::string> queryResult = queryManager.process(queryString);
        std::vector<std::string> expectedResult = {"SemanticError"};

        REQUIRE(queryResult == expectedResult);
    }

    SECTION("Procedure Invalid value Test") {
        std::string queryString = "procedure p;\nSelect p  with p.value = 1";

        std::vector<std::string> queryResult = queryManager.process(queryString);
        std::vector<std::string> expectedResult = {"SemanticError"};

        REQUIRE(queryResult == expectedResult);
    }

    SECTION("Call Invalid varName Test") {
        std::string queryString = "call c;\nSelect c  with \"x\" = c.varName";

        std::vector<std::string> queryResult = queryManager.process(queryString);
        std::vector<std::string> expectedResult = {"SemanticError"};

        REQUIRE(queryResult == expectedResult);
    }

    SECTION("Call Invalid value Test") {
        std::string queryString = "call c;\nSelect c  with 2 = c.value";

        std::vector<std::string> queryResult = queryManager.process(queryString);
        std::vector<std::string> expectedResult = {"SemanticError"};

        REQUIRE(queryResult == expectedResult);
    }

    SECTION("Variable Invalid procName Test") {
        std::string queryString = "variable v;\nSelect v  with \"var\" = v.procName";

        std::vector<std::string> queryResult = queryManager.process(queryString);
        std::vector<std::string> expectedResult = {"SemanticError"};

        REQUIRE(queryResult == expectedResult);
    }

    SECTION("Variable Invalid value Test") {
        std::string queryString = "variable v;\nSelect v  with 10 = v.value";

        std::vector<std::string> queryResult = queryManager.process(queryString);
        std::vector<std::string> expectedResult = {"SemanticError"};

        REQUIRE(queryResult == expectedResult);
    }

    SECTION("Variable Invalid stmt# Test") {
        std::string queryString = "variable v;\nSelect v  with 10 = v.stmt#";

        std::vector<std::string> queryResult = queryManager.process(queryString);
        std::vector<std::string> expectedResult = {"SemanticError"};

        REQUIRE(queryResult == expectedResult);
    }

    SECTION("Read Invalid procName Test") {
        std::string queryString = "read re;\nSelect re with re.procName = \"xyz\"";

        std::vector<std::string> queryResult = queryManager.process(queryString);
        std::vector<std::string> expectedResult = {"SemanticError"};

        REQUIRE(queryResult == expectedResult);
    }

    SECTION("Read Invalid value Test") {
        std::string queryString = "read re;\nSelect re with re.value = 1";

        std::vector<std::string> queryResult = queryManager.process(queryString);
        std::vector<std::string> expectedResult = {"SemanticError"};

        REQUIRE(queryResult == expectedResult);
    }

    SECTION("Print Invalid procName Test") {
        std::string queryString = "procedure p; print pn;\nSelect pn with p.procName = pn.procName";

        std::vector<std::string> queryResult = queryManager.process(queryString);
        std::vector<std::string> expectedResult = {"SemanticError"};

        REQUIRE(queryResult == expectedResult);
    }

    SECTION("Print Invalid value Test") {
        std::string queryString = "procedure p; print pn;\nSelect pn with p.procName = pn.value";

        std::vector<std::string> queryResult = queryManager.process(queryString);
        std::vector<std::string> expectedResult = {"SemanticError"};

        REQUIRE(queryResult == expectedResult);
    }

    SECTION("Constant Invalid procName Test") {
        std::string queryString = "constant c;\nSelect c with c.procName = 1";

        std::vector<std::string> queryResult = queryManager.process(queryString);
        std::vector<std::string> expectedResult = {"SemanticError"};

        REQUIRE(queryResult == expectedResult);
    }

    SECTION("Constant Invalid varName Test") {
        std::string queryString = "constant c;\nSelect c with c.varName = 1";

        std::vector<std::string> queryResult = queryManager.process(queryString);
        std::vector<std::string> expectedResult = {"SemanticError"};

        REQUIRE(queryResult == expectedResult);
    }

    SECTION("Constant Invalid stmt# Test") {
        std::string queryString = "constant c;\nSelect c with c.stmt#= 1";

        std::vector<std::string> queryResult = queryManager.process(queryString);
        std::vector<std::string> expectedResult = {"SemanticError"};

        REQUIRE(queryResult == expectedResult);
    }

    SECTION("Stmt Invalid procName Test") {
        std::string queryString = "stmt s; assign a;\nSelect s with s.procName = a.stmt#";

        std::vector<std::string> queryResult = queryManager.process(queryString);
        std::vector<std::string> expectedResult = {"SemanticError"};

        REQUIRE(queryResult == expectedResult);
    }

    SECTION("Stmt Invalid varName Test") {
        std::string queryString = "stmt s; assign a;\nSelect s with s.varName = a.stmt#";

        std::vector<std::string> queryResult = queryManager.process(queryString);
        std::vector<std::string> expectedResult = {"SemanticError"};

        REQUIRE(queryResult == expectedResult);
    }

    SECTION("Stmt Invalid value Test") {
        std::string queryString = "stmt s; assign a;\nSelect s with s.value = a.stmt#";

        std::vector<std::string> queryResult = queryManager.process(queryString);
        std::vector<std::string> expectedResult = {"SemanticError"};

        REQUIRE(queryResult == expectedResult);
    }

    SECTION("While Invalid procName Test") {
        std::string queryString = "if if; while w;\nSelect if with if.stmt# = w.procName";

        std::vector<std::string> queryResult = queryManager.process(queryString);
        std::vector<std::string> expectedResult = {"SemanticError"};

        REQUIRE(queryResult == expectedResult);
    }

    SECTION("While Invalid varName Test") {
        std::string queryString = "if if; while w;\nSelect if with if.stmt# = w.varName";

        std::vector<std::string> queryResult = queryManager.process(queryString);
        std::vector<std::string> expectedResult = {"SemanticError"};

        REQUIRE(queryResult == expectedResult);
    }

    SECTION("While Invalid value Test") {
        std::string queryString = "if if; while w;\nSelect if with if.stmt# = w.value";

        std::vector<std::string> queryResult = queryManager.process(queryString);
        std::vector<std::string> expectedResult = {"SemanticError"};

        REQUIRE(queryResult == expectedResult);
    }

    SECTION("If Invalid procName Test") {
        std::string queryString = "if if; while w;\nSelect if with if.procName = w.stmt#";

        std::vector<std::string> queryResult = queryManager.process(queryString);
        std::vector<std::string> expectedResult = {"SemanticError"};

        REQUIRE(queryResult == expectedResult);
    }

    SECTION("If Invalid varName Test") {
        std::string queryString = "if if; while w;\nSelect if with if.varName = w.stmt#";

        std::vector<std::string> queryResult = queryManager.process(queryString);
        std::vector<std::string> expectedResult = {"SemanticError"};

        REQUIRE(queryResult == expectedResult);
    }

    SECTION("If Invalid value Test") {
        std::string queryString = "if if; while w;\nSelect if with if.value = w.stmt#";

        std::vector<std::string> queryResult = queryManager.process(queryString);
        std::vector<std::string> expectedResult = {"SemanticError"};

        REQUIRE(queryResult == expectedResult);
    }

    SECTION("Assign Invalid procName Test") {
        std::string queryString = "stmt s; assign a;\nSelect s with s.stmt# = a.procName";

        std::vector<std::string> queryResult = queryManager.process(queryString);
        std::vector<std::string> expectedResult = {"SemanticError"};

        REQUIRE(queryResult == expectedResult);
    }

    SECTION("Assign Invalid varName Test") {
        std::string queryString = "stmt s; assign a;\nSelect s with s.stmt# = a.varName";

        std::vector<std::string> queryResult = queryManager.process(queryString);
        std::vector<std::string> expectedResult = {"SemanticError"};

        REQUIRE(queryResult == expectedResult);
    }

    SECTION("Assign Invalid value Test") {
        std::string queryString = "stmt s; assign a;\nSelect s with s.stmt# = a.value";

        std::vector<std::string> queryResult = queryManager.process(queryString);
        std::vector<std::string> expectedResult = {"SemanticError"};

        REQUIRE(queryResult == expectedResult);
    }

    SECTION("Invalid procName = INTEGER Test") {
        std::string queryString = "procedure p;\nSelect p  with p.procName = 10";

        std::vector<std::string> queryResult = queryManager.process(queryString);
        std::vector<std::string> expectedResult = {"SemanticError"};

        REQUIRE(queryResult == expectedResult);
    }

    SECTION("Invalid varName = INTEGER Test") {
        std::string queryString = "variable v;\nSelect v  with 11 = v.varName";

        std::vector<std::string> queryResult = queryManager.process(queryString);
        std::vector<std::string> expectedResult = {"SemanticError"};

        REQUIRE(queryResult == expectedResult);
    }

    SECTION("Invalid value = NAME Test") {
        std::string queryString = "constant c;\nSelect c with c.value = \"x1\"";

        std::vector<std::string> queryResult = queryManager.process(queryString);
        std::vector<std::string> expectedResult = {"SemanticError"};

        REQUIRE(queryResult == expectedResult);
    }

    SECTION("Invalid stmt# = NAME Test") {
        std::string queryString = "stmt s; assign a;\nSelect s with s.stmt# = \"x\"";

        std::vector<std::string> queryResult = queryManager.process(queryString);
        std::vector<std::string> expectedResult = {"SemanticError"};

        REQUIRE(queryResult == expectedResult);
    }
}
