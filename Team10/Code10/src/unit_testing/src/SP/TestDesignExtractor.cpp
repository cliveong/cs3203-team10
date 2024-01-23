#include "catch.hpp"
#include "pkb/relationship_managers/ModifiesS.h"
#include "pkb/relationship_managers/UsesS.h"
#include "sp/DesignExtractor.h"
#include "sp/Parsers/Parser.h"
#include "sp/Tokenizer.h"

std::shared_ptr<Storage> runExtractor(std::string input) {
    Tokenizer tokenizer(input);
    ParserContext context(tokenizer.tokenize());

    Parser parser(tokenizer.tokenize());
    std::shared_ptr<Storage> storage = make_shared<Storage>();
    WriteFacade writeFacade = WriteFacade(storage);
    std::shared_ptr<ASTNode> rootnode = parser.parse();
    DesignExtractor designExtractor = DesignExtractor(writeFacade, rootnode);
    designExtractor.extractUses(rootnode);
    return storage;
}

template <typename T>
void printVectorWithBrackets(const std::vector<T>& vec) {
    std::cout << "[";
    for (size_t i = 0; i < vec.size(); ++i) {
        std::cout << vec[i];
        if (i < vec.size() - 1) {
            std::cout << ", ";
        }
    }
    std::cout << "]" << std::endl;
}

TEST_CASE("Testing extraction of uses") {
    // std::string input = "procedure main { a =3 + w; read b; if (3+c == d) then
    // { e=3; print f; } else { y = z+v; while(x < 2) {h = p; read m; j = k +
    // l;}}}";

    SECTION("Multiple assign, print, read statements") {
        std::string input =
            "procedure main { a = 3 + y + w; print b; read x; p = 3;}";
        std::shared_ptr<Storage> storage = runExtractor(input);

        std::vector<std::string> res = storage->getUses(1);
        REQUIRE(res.size() == 2);
        REQUIRE(res[0] == "y");
        REQUIRE(res[1] == "w");
        res = storage->getUses(2);
        REQUIRE(res[0] == "b");
        res = storage->getUses(3);
        REQUIRE(res.size() == 0);
        res = storage->getUses(4);
        REQUIRE(res.size() == 0);
    }

    SECTION("If statements") {
        std::string input =
            "procedure main { if(a + b <2) then {c = d + e; print f; } else {g = "
            "h+ i; read j; print h;} }";
        std::shared_ptr<Storage> storage = runExtractor(input);

        std::vector<std::string> res = storage->getUses(1);
        REQUIRE(res.size() == 7);
        res = storage->getUses(2);
        REQUIRE(res.size() == 2);
        res = storage->getUses(3);
        REQUIRE(res.size() == 1);
        res = storage->getUses(4);
        REQUIRE(res.size() == 2);
        res = storage->getUses(6);
        REQUIRE(res.size() == 1);
    }

    SECTION("Nested If statements") {
        std::string input =
            "procedure main { if(a + b <2) then {c = d + e; print f; } else {g = "
            "h+ i; if (3 < i) then { read j; print h;} else {x = y+z;} } }";
        std::shared_ptr<Storage> storage = runExtractor(input);

        std::vector<std::string> res = storage->getUses(1);
        REQUIRE(res.size() == 9);
        res = storage->getUses(2);
        REQUIRE(res.size() == 2);
        res = storage->getUses(3);
        REQUIRE(res.size() == 1);
        res = storage->getUses(4);
        REQUIRE(res.size() == 2);
        res = storage->getUses(5);
        REQUIRE(res.size() == 4);
        res = storage->getUses(7);
        REQUIRE(res.size() == 1);
        res = storage->getUses(8);
        REQUIRE(res.size() == 2);
    }

    SECTION("Nested while and if statements") {
        std::string input =
            "procedure main { while(a + b == c + 2) { if (d >= 3) then { print e; "
            "f = g+h;} else { i = j;} } h = j + k; }";
        std::shared_ptr<Storage> storage = runExtractor(input);

        std::vector<std::string> res = storage->getUses(1);
        REQUIRE(res.size() == 8);
        res = storage->getUses(2);
        REQUIRE(res.size() == 5);
        res = storage->getUses(3);
        REQUIRE(res.size() == 1);
        res = storage->getUses(4);
        REQUIRE(res.size() == 2);
        res = storage->getUses(5);
        REQUIRE(res.size() == 1);
        res = storage->getUses(6);
        REQUIRE(res.size() == 2);
    }

    SECTION("Double Nested while and if statements") {
        std::string input =
            "procedure main { while(a + b == c + 2) { if (d >= 3) then { print e; "
            "f = g+h;} else {while (l == m) { print n;} i = j;} } h = j + k; }";
        std::shared_ptr<Storage> storage = runExtractor(input);

        std::vector<std::string> res = storage->getUses(1);
        REQUIRE(res.size() == 11);
        res = storage->getUses(2);
        REQUIRE(res.size() == 8);
        res = storage->getUses(3);
        REQUIRE(res.size() == 1);
        res = storage->getUses(4);
        REQUIRE(res.size() == 2);
        res = storage->getUses(5);
        REQUIRE(res.size() == 3);
        res = storage->getUses(6);
        REQUIRE(res.size() == 1);
        res = storage->getUses(7);
        REQUIRE(res.size() == 1);
        res = storage->getUses(8);
        REQUIRE(res.size() == 2);
    }

    SECTION("Uses proc") {
        std::string input = "procedure main { print a;  b = 1 + c + d;}";
        std::shared_ptr<Storage> storage = runExtractor(input);

        std::vector<std::string> res = storage->getUsesProc("main");
        REQUIRE(res.size() == 3);
    }

    SECTION("Uses proc with one calls") {
        std::string input =
            "procedure main { print a;  b = 1 + c; call next;} procedure next "
            "{read d; print e; f = g + i;}";
        std::shared_ptr<Storage> storage = runExtractor(input);

        std::vector<std::string> res = storage->getUsesProc("main");
        REQUIRE(res.size() == 5);

        res = storage->getUsesProc("next");
        REQUIRE(res.size() == 3);
    }

    SECTION("Uses proc with two calls") {
        std::string input =
            "procedure main { print a;  b = 1 + c; call next;} procedure next "
            "{read d; call third; print e; f = g + i;} procedure third {j = k;}";
        std::shared_ptr<Storage> storage = runExtractor(input);

        std::vector<std::string> res = storage->getUsesProc("main");
        REQUIRE(res.size() == 6);

        res = storage->getUsesProc("next");
        REQUIRE(res.size() == 4);

        res = storage->getUsesProc("third");
        REQUIRE(res.size() == 1);
    }
}

std::shared_ptr<Storage> runExtractorModifies(std::string input) {
    Tokenizer tokenizer(input);

    Parser parser(tokenizer.tokenize());
    std::shared_ptr<Storage> storage = make_shared<Storage>();
    WriteFacade writeFacade = WriteFacade(storage);
    const std::shared_ptr<ASTNode> rootnode = parser.parse();
    DesignExtractor designExtractor = DesignExtractor(writeFacade, rootnode);
    designExtractor.extractModifies(rootnode);
    return storage;
}

TEST_CASE("Test extract modifies") {
    SECTION("Modifies proc") {
        std::string input = "procedure main { read a;  b = 1 + c + d;}";
        std::shared_ptr<Storage> storage = runExtractorModifies(input);

        std::vector<std::string> res = storage->getModifiesProc("main");
        REQUIRE(res.size() == 2);
    }

    SECTION("Modifies proc with one calls") {
        std::string input =
            "procedure main { read a;  b = 1 + c; call next;} procedure next {read "
            "d; print e; f = g + i;}";
        std::shared_ptr<Storage> storage = runExtractorModifies(input);

        std::vector<std::string> res = storage->getModifiesProc("main");
        REQUIRE(res.size() == 4);

        res = storage->getModifiesProc("next");
        REQUIRE(res.size() == 2);
    }

    SECTION("Modifies proc with two calls") {
        std::string input =
            "procedure main { read a;  b = 1 + c; call next;} procedure next {read "
            "d; call third; print e; f = g + i;} procedure third {j = k;}";
        std::shared_ptr<Storage> storage = runExtractorModifies(input);

        std::vector<std::string> res = storage->getModifiesProc("main");
        REQUIRE(res.size() == 5);

        res = storage->getModifiesProc("next");
        REQUIRE(res.size() == 3);

        res = storage->getModifiesProc("third");
        REQUIRE(res.size() == 1);
    }

    SECTION("Multiple assign, print, read statements") {
        std::string input =
            "procedure main { a = 3 + y + w; print b; read x; p = 3;}";
        std::shared_ptr<Storage> storage = runExtractorModifies(input);

        std::vector<std::string> res = storage->getModifies(1);
        REQUIRE((res.size() == 1 && res[0] == "a"));

        res = storage->getModifies(2);
        REQUIRE(res.size() == 0);

        res = storage->getModifies(3);
        REQUIRE((res.size() == 1 && res[0] == "x"));

        res = storage->getModifies(4);
        REQUIRE((res.size() == 1 && res[0] == "p"));
    }

    SECTION("If statements") {
        std::string input =
            "procedure main { if(a + b <2) then {c = d + e; read f; } else {g = h+ "
            "i; read j; } }";
        std::shared_ptr<Storage> storage = runExtractorModifies(input);

        std::vector<std::string> res = storage->getModifies(1);
        REQUIRE(res.size() == 4);
        res = storage->getModifies(2);
        REQUIRE((res.size() == 1 && res[0] == "c"));

        res = storage->getModifies(3);
        REQUIRE((res.size() == 1 && res[0] == "f"));

        res = storage->getModifies(4);
        REQUIRE((res.size() == 1 && res[0] == "g"));

        res = storage->getModifies(5);
        REQUIRE((res.size() == 1 && res[0] == "j"));
    }

    SECTION("Nested If statements") {
        std::string input =
            "procedure main { if(a + b <2) then {c = d + e; } else {g = h+ i; if "
            "(3 < i) then { read j; print h;} else {x = y+z;} } }";
        std::shared_ptr<Storage> storage = runExtractorModifies(input);

        std::vector<std::string> res = storage->getModifies(1);
        REQUIRE(res.size() == 4);
        res = storage->getModifies(2);
        REQUIRE(res.size() == 1);
        res = storage->getModifies(3);
        REQUIRE(res.size() == 1);
        res = storage->getModifies(4);
        REQUIRE(res.size() == 2);
        res = storage->getModifies(5);
        REQUIRE(res.size() == 1);
        res = storage->getModifies(7);
        REQUIRE(res.size() == 1);
    }

    SECTION("Nested while and if statements") {
        std::string input =
            "procedure main { x=y-z; while(a + b == c + 2) { if (d >= 3) then { "
            "read e; f = g+h;} else { i = j;} } h = j + k; }";
        std::shared_ptr<Storage> storage = runExtractorModifies(input);

        std::vector<std::string> res = storage->getModifies(1);
        REQUIRE(res[0] == "x");
        res = storage->getModifies(2);
        REQUIRE(res.size() == 3);
        res = storage->getModifies(3);
        REQUIRE(res.size() == 3);
        res = storage->getModifies(4);
        REQUIRE(res[0] == "e");
        res = storage->getModifies(5);
        REQUIRE(res[0] == "f");
        res = storage->getModifies(6);
        REQUIRE(res.size() == 1);
        res = storage->getModifies(7);
        REQUIRE(res.size() == 1);
    }

    SECTION("Double Nested while and if statements") {
        std::string input =
            "procedure main { while(a + b == c + 2) { if (d >= 3) then { read e;} "
            "else {while (l == m) {i = j;} read n;} } }";
        std::shared_ptr<Storage> storage = runExtractorModifies(input);

        std::vector<std::string> res = storage->getModifies(1);
        REQUIRE(res.size() == 3);
        res = storage->getModifies(2);
        REQUIRE(res.size() == 3);
        res = storage->getModifies(3);
        REQUIRE(res.size() == 1);
        res = storage->getModifies(4);
        REQUIRE(res.size() == 1);
        res = storage->getModifies(5);
        REQUIRE(res.size() == 1);
        res = storage->getModifies(6);
        REQUIRE(res.size() == 1);
    }
}

std::shared_ptr<Storage> runExtractorCFG(std::string input) {
    Tokenizer tokenizer(input);

    Parser parser(tokenizer.tokenize());
    std::shared_ptr<Storage> storage = make_shared<Storage>();
    WriteFacade writeFacade = WriteFacade(storage);
    const std::shared_ptr<ASTNode> rootnode = parser.parse();
    DesignExtractor designExtractor = DesignExtractor(writeFacade, rootnode);
    designExtractor.extractCFG(rootnode);
    return storage;
}

TEST_CASE("Test CFG") {
    std::string input =
        R"(procedure main { 
                if (x<1) then {
                    read x; 
                    while (x==2) {
                        print m; 
                        x = 1; 
                    if (x==2) then {
                        print smth;
                    } else {
                    read smth;
                    }
                        
                   }
                } else {print y;} 
                read p;
        }

        procedure next {
             print x;
             while(x<2) {
               call smth;
            }
        }

)";
    std::shared_ptr<Storage> storage = runExtractorCFG(input);
}

std::shared_ptr<Storage> runExtractorFollowsStar(std::string input) {
    Tokenizer tokenizer(input);

    Parser parser(tokenizer.tokenize());
    std::shared_ptr<Storage> storage = make_shared<Storage>();
    WriteFacade writeFacade = WriteFacade(storage);
    const std::shared_ptr<ASTNode> rootnode = parser.parse();
    DesignExtractor designExtractor = DesignExtractor(writeFacade, rootnode);
    designExtractor.extractFollows(rootnode);
    return storage;
}

TEST_CASE("Testing extraction of follows*") {
    SECTION("Individual statements base case") {
        std::string input = "procedure main { print x; x=1; print y;}";
        std::shared_ptr<Storage> storage = runExtractorFollowsStar(input);
        /*
        std::vector<std::vector<int>> followsStarKeys = storage->getAllFollows();
        REQUIRE(followsStarKeys.size() == 2);
        */

        bool hasFollows = storage->hasFollows();
        REQUIRE(hasFollows);
        std::vector<int> followsRs = storage->getFollows(1, true);
        REQUIRE(followsRs[0] == 2);
        REQUIRE(followsRs[1] == 3);

        followsRs = storage->getFollows(2, true);
        REQUIRE(followsRs[0] == 3);
    }
    SECTION("one loop") {
        std::string input =
            "procedure main { read y; if (c>3) then { read a; } else { read b; } "
            "print x;}";
        std::shared_ptr<Storage> storage = runExtractorFollowsStar(input);

        /*
        std::vector<std::vector<int>> followsStarKeys = storage->getAllFollows();
        REQUIRE(followsStarKeys.size() == 2);
        */

        bool hasFollows = storage->hasFollows();
        REQUIRE(hasFollows);

        std::vector<int> followsRs = storage->getFollows(1, true);

        REQUIRE(followsRs[0] == 2);
        REQUIRE(followsRs[1] == 5);

        followsRs = storage->getFollows(2, true);
        REQUIRE(followsRs[0] == 5);
    }
    SECTION("multiple loops sequence") {
        std::string input =
            "procedure main { if (c>3) then { read a; } else { print x; } while "
            "(v>b) { print do; } if (a<b) then { print smile; } else { read "
            "something; } }";
        std::shared_ptr<Storage> storage = runExtractorFollowsStar(input);

        /*
        std::vector<std::vector<int>> followsStarKeys = storage->getAllFollows();
        REQUIRE(followsStarKeys.size() == 2);
        */

        std::vector<int> followsRs = storage->getFollows(1, true);
        REQUIRE(followsRs[0] == 4);
        REQUIRE(followsRs[1] == 6);

        bool hasFollows = storage->hasFollows();
        REQUIRE(hasFollows);

        followsRs = storage->getFollows(4, true);
        REQUIRE(followsRs[0] == 6);
    }

    SECTION("multiple loops nested") {
        std::string input =
            "procedure main { if (c>3) then { read a; } else { while (v>b) { if "
            "(a<b) then { print smile; } else { read something; } } print x; } }";
        std::shared_ptr<Storage> storage = runExtractorFollowsStar(input);

        /*std::vector<std::vector<int>> followsStarKeys = storage->getAllFollows();



        std::cout << std::endl;
        REQUIRE(followsStarKeys.size() == 1);*/

        bool hasFollows = storage->hasFollows();
        REQUIRE(hasFollows);

        std::vector<int> followsRs = storage->getFollows(3, true);
        REQUIRE(followsRs[0] == 7);
    }
}

std::shared_ptr<Storage> runExtractorEntities(std::string input) {
    Tokenizer tokenizer(input);

    Parser parser(tokenizer.tokenize());
    std::shared_ptr<Storage> storage = make_shared<Storage>();
    WriteFacade writeFacade = WriteFacade(storage);
    const std::shared_ptr<ASTNode> rootnode = parser.parse();
    DesignExtractor designExtractor = DesignExtractor(writeFacade, rootnode);
    designExtractor.extractEntities(rootnode);
    return storage;
}

TEST_CASE("Test extraction of entities") {
    std::string input = "procedure main { read rrr; print zzz; }";
    std::shared_ptr<Storage> storage = runExtractorEntities(input);
}

std::shared_ptr<Storage> runExtractorPattern(std::string input) {
    Tokenizer tokenizer(input);

    Parser parser(tokenizer.tokenize());
    std::shared_ptr<Storage> storage = make_shared<Storage>();
    WriteFacade writeFacade = WriteFacade(storage);
    const std::shared_ptr<ASTNode> rootnode = parser.parse();
    DesignExtractor designExtractor = DesignExtractor(writeFacade, rootnode);
    designExtractor.extractPattern(rootnode);
    return storage;
}

TEST_CASE("Test extraction of if pattern") {
    std::string input =
        "procedure main { if (y + 2 < x)  then { print x;} else { read x; if (p "
        "== q) then { print m; } else { x=2;}}  }";
    std::shared_ptr<Storage> storage = runExtractorPattern(input);

    std::string str = "x";
    REQUIRE(storage->getIfPattern(str).size() == 1);
    REQUIRE(storage->getIfPattern(str)[0] == 1);

    str = "q";
    REQUIRE(storage->getIfPattern(str)[0] == 4);
}

TEST_CASE("Testing extraction of Assign Pattern") {
    SECTION("base case assign pattern, one statement") {
        std::string input =
            "procedure main { while (p==2) { x = 42; x = y + 42 - f; p = 42; }}";
        std::shared_ptr<Storage> storage = runExtractorPattern(input);

        ASTNode node = ASTNode("const", "42", 2);
        std::string str = "x";
        REQUIRE(storage->getAssignPattern(str, node) == std::vector<int>{2, 3});
    }

    SECTION("complex assign pattern") {
        std::string input =
            "procedure main { x = 42; x = y + 42 - f; x = c + 42 / 2; }";
        std::shared_ptr<Storage> storage = runExtractorPattern(input);

        std::string str = "x";
        ASTNode node("/", "/", 3);
        node.addChild(std::make_shared<ASTNode>("const", "42", 3));
        node.addChild(std::make_shared<ASTNode>("const", "2", 3));
        REQUIRE(storage->getAssignPattern(str, node) == std::vector<int>{3});
    }
}

std::shared_ptr<Storage> runExtractorParentStar(std::string input) {
    Tokenizer tokenizer(input);

    Parser parser(tokenizer.tokenize());
    std::shared_ptr<Storage> storage = make_shared<Storage>();
    WriteFacade writeFacade = WriteFacade(storage);
    const std::shared_ptr<ASTNode> rootnode = parser.parse();
    DesignExtractor designExtractor = DesignExtractor(writeFacade, rootnode);
    designExtractor.extractParents(rootnode);
    return storage;
}

TEST_CASE("Testing extraction of Parent*") {
    /*
    SECTION("two level nesting") {
          std::string input = "procedure main { read y; if (c>3) then { read a; }
    else { read b; while (9>0) {read something;} } print x;}";
          std::shared_ptr<Storage> storage = runExtractorParentStar(input);

          std::vector<std::vector<int>> parentStarKeys =
    storage->getAllParents(true); REQUIRE(parentStarKeys.size() == 2);

          std::vector<int> parentStarRs = storage->getParents(2, true);
          REQUIRE(parentStarRs[0] == 3);
          REQUIRE(parentStarRs[1] == 4);
          REQUIRE(parentStarRs[2] == 5);
          REQUIRE(parentStarRs[3] == 6);

          parentStarRs = storage->getParents(5, true);
          REQUIRE(parentStarRs[0] == 6);
      }
    */

    SECTION("three level nesting") {
        std::string input =
            "procedure main { read y; if (c>3) then { read a; } else { read b; "
            "while (9>0) {read something; while (m<n) { print nothing; } } } }";
        std::shared_ptr<Storage> storage = runExtractorParentStar(input);
        bool hasParentStar = storage->hasParentsStar();
        REQUIRE(hasParentStar);
        std::vector<int> parentStarRs = storage->getParents(2, true);
        REQUIRE(parentStarRs.size() == 6);
        REQUIRE(parentStarRs[0] == 3);
        REQUIRE(parentStarRs[1] == 4);
        REQUIRE(parentStarRs[2] == 5);
        REQUIRE(parentStarRs[3] == 6);
        REQUIRE(parentStarRs[4] == 7);
        REQUIRE(parentStarRs[5] == 8);

        parentStarRs = storage->getParents(5, true);
        REQUIRE(parentStarRs.size() == 3);
        REQUIRE(parentStarRs[0] == 6);
        REQUIRE(parentStarRs[1] == 7);
        REQUIRE(parentStarRs[2] == 8);

        parentStarRs = storage->getParents(7, true);
        REQUIRE(parentStarRs[0] == 8);
    }

    SECTION("four level nesting") {
        std::string input =
            "procedure main { read y; if (c>3) then { read a; } else { read b; "
            "while (9>0) {read something; while (m<n) { print nothing; while (i "
            "== "
            "0) { read template; } } } } }";
        std::shared_ptr<Storage> storage = runExtractorParentStar(input);
        /*std::vector<std::vector<int>> parentStarKeys =
        storage->getAllParents(true); REQUIRE(parentStarKeys.size() == 4);
        */

        SECTION("four level nesting") {
            std::string input =
                "procedure main { read y; if (c>3) then { read a; } else { read b; "
                "while (9>0) {read something; while (m<n) { print nothing; while "
                "(i == 0) { read template; } } } } }";
            std::shared_ptr<Storage> storage = runExtractorParentStar(input);
            bool hasParentStar = storage->hasParentsStar();
            REQUIRE(hasParentStar);
            std::vector<int> parentStarRs = storage->getParents(2, true);
            REQUIRE(parentStarRs[0] == 3);
            REQUIRE(parentStarRs[1] == 4);
            REQUIRE(parentStarRs[2] == 5);
            REQUIRE(parentStarRs[3] == 6);
            REQUIRE(parentStarRs[4] == 7);
            REQUIRE(parentStarRs[5] == 8);
            REQUIRE(parentStarRs[6] == 9);
            REQUIRE(parentStarRs[7] == 10);

            parentStarRs = storage->getParents(5, true);
            REQUIRE(parentStarRs[0] == 6);
            REQUIRE(parentStarRs[1] == 7);
            REQUIRE(parentStarRs[2] == 8);
            REQUIRE(parentStarRs[3] == 9);
            REQUIRE(parentStarRs[4] == 10);

            parentStarRs = storage->getParents(7, true);
            REQUIRE(parentStarRs[0] == 8);
            REQUIRE(parentStarRs[1] == 9);
            REQUIRE(parentStarRs[2] == 10);

            parentStarRs = storage->getParents(9, true);
            REQUIRE(parentStarRs[0] == 10);
        }
    }
}

std::shared_ptr<Storage> runExtractorParents(std::string input) {
    Tokenizer tokenizer(input);

    Parser parser(tokenizer.tokenize());
    std::shared_ptr<Storage> storage = make_shared<Storage>();
    WriteFacade writeFacade = WriteFacade(storage);
    const std::shared_ptr<ASTNode> rootnode = parser.parse();
    DesignExtractor designExtractor = DesignExtractor(writeFacade, rootnode);
    designExtractor.extractParents(rootnode);
    return storage;
}

TEST_CASE("Testing extraction of Parent") {
    SECTION("tqo level nesting, back to back statements ") {
        std::string input =
            "procedure main { if (a>1) then { b = c; a = 1; } else { while (x>1) { "
            "y = z; } if (a<3) then { b = 2; } else { print g;} } }";
        std::shared_ptr<Storage> storage = runExtractorParents(input);

        /*
        std::vector<std::vector<int>> parentKeys = storage->getAllParents(false);
        REQUIRE(parentKeys.size() == 3);
        */

        bool hasParents = storage->hasParents();
        REQUIRE(hasParents);
        std::vector<int> parentRs = storage->getParents(1, false);
        REQUIRE(parentRs[0] == 2);
        REQUIRE(parentRs[1] == 3);
        REQUIRE(parentRs[2] == 4);
        REQUIRE(parentRs[3] == 6);

        parentRs = storage->getParents(4, false);
        REQUIRE(parentRs[0] == 5);
        parentRs = storage->getParents(4, false);
        REQUIRE(parentRs[0] == 5);

        parentRs = storage->getParents(6, false);
        REQUIRE(parentRs[0] == 7);
        REQUIRE(parentRs[1] == 8);
    }
}

std::shared_ptr<Storage> runExtractorCalls(std::string input) {
    Tokenizer tokenizer(input);

    Parser parser(tokenizer.tokenize());
    std::shared_ptr<Storage> storage = make_shared<Storage>();
    WriteFacade writeFacade = WriteFacade(storage);
    const std::shared_ptr<ASTNode> rootnode = parser.parse();
    DesignExtractor designExtractor = DesignExtractor(writeFacade, rootnode);
    designExtractor.extractCallsStar(rootnode);
    designExtractor.extractCalls(rootnode);
    designExtractor.extractEntities(rootnode);
    return storage;
}

TEST_CASE("Testing extraction of Calls") {
    SECTION(" basic calls ") {
        std::string input = "procedure a { call b; } procedure b { print why; }";
        std::shared_ptr<Storage> storage = runExtractorCalls(input);

        bool callsVals = storage->hasCalls();
        REQUIRE(callsVals);

        std::string callsname = storage->getStmtToProcName(1);
        REQUIRE(callsname == "b");
    }

    SECTION(" double calls ") {
        std::string input = "procedure a { print who; call b; } procedure b { print why; call c; }procedure c {x = x + 1;}";
        std::shared_ptr<Storage> storage = runExtractorCalls(input);

        bool callsVals = storage->hasCalls();
        REQUIRE(callsVals);

        std::string callsname = storage->getStmtToProcName(2);
        REQUIRE(callsname == "b");

        std::string callsname1 = storage->getStmtToProcName(4);
        REQUIRE(callsname1 == "c");
    }

    SECTION(" sequential calls ") {
        std::string input =
            " procedure a { read hi; call b; } "
            "procedure b { read where; call c; } "
            "procedure c { read who; call d; while (zyx < abc) "
            "{ read where; print why; assign = assign + 3; call e; } } procedure d { read what; } "
            "procedure e { y = y + 1; } procedure f { read wombats; } ";
        std::shared_ptr<Storage> storage = runExtractorCalls(input);

        std::string callsname = storage->getStmtToProcName(2);
        REQUIRE(callsname == "b");

        std::string callsname3 = storage->getStmtToProcName(4);
        REQUIRE(callsname3 == "c");

        std::string callsname1 = storage->getStmtToProcName(6);
        REQUIRE(callsname1 == "d");

        std::string callsname2 = storage->getStmtToProcName(11);
        REQUIRE(callsname2 == "e");
    }

    SECTION(" repeated calls ") {
        std::string input =
            " procedure a { read hi; call b; call b; call b; } "
            "procedure b { read where; call c; call d; } "
            "procedure c { read who; call d; call e; } procedure d { read what; } "
            "procedure e { y = y + 1; } procedure f { read wombats; } ";

        std::shared_ptr<Storage> storage = runExtractorCalls(input);

        bool callsVals = storage->hasCalls();
        REQUIRE(callsVals);

        std::string callsname = storage->getStmtToProcName(2);
        REQUIRE(callsname == "b");

        std::string callsname1 = storage->getStmtToProcName(3);
        REQUIRE(callsname1 == "b");

        std::string callsname2 = storage->getStmtToProcName(4);
        REQUIRE(callsname2 == "b");

        std::string callsname3 = storage->getStmtToProcName(6);
        REQUIRE(callsname3 == "c");

        std::string callsname4 = storage->getStmtToProcName(7);
        REQUIRE(callsname4 == "d");

        std::string callsname5 = storage->getStmtToProcName(9);
        REQUIRE(callsname5 == "d");

        std::string callsname6 = storage->getStmtToProcName(10);
        REQUIRE(callsname6 == "e");
    }

    SECTION(" repeated and nested calls ") {
        std::string input =
            "procedure a { read hi; call b; call d; } "
            "procedure b { read where; call c; } "
            "procedure c { read who; call d; while (zyx < abc) "
            "{ read where; print why; assign = assign + 3; call e; "
            "if (if > then) then { while (3 < a ) { a = 1; } "
            "read rrr; call f; } else { t = 322303; call f; } } } "
            "procedure d { read what; } procedure e { y = y + 1; } "
            "procedure f { read whomst; } ";
        std::shared_ptr<Storage> storage = runExtractorCalls(input);

        bool callsVals = storage->hasCalls();
        REQUIRE(callsVals);

        std::string callsname = storage->getStmtToProcName(2);
        REQUIRE(callsname == "b");

        std::string callsname1 = storage->getStmtToProcName(3);
        REQUIRE(callsname1 == "d");

        std::string callsname2 = storage->getStmtToProcName(5);
        REQUIRE(callsname2 == "c");

        std::string callsname3 = storage->getStmtToProcName(7);
        REQUIRE(callsname3 == "d");

        std::string callsname4 = storage->getStmtToProcName(12);
        REQUIRE(callsname4 == "e");

        std::string callsname5 = storage->getStmtToProcName(17);
        REQUIRE(callsname5 == "f");

        std::string callsname6 = storage->getStmtToProcName(19);
        REQUIRE(callsname6 == "f");
    }

    SECTION("No Calls in the program") {
        std::string input = "procedure a { print x; } procedure b { x = 1; }";
        std::shared_ptr<Storage> storage = runExtractorCalls(input);

        // no calls in the program
        bool callsVals = storage->hasCalls();
        REQUIRE(!callsVals);
    }

    SECTION("Calls between multiple procedures") {
        std::string input = "procedure a { call b; } procedure b { call c; } procedure c { print hi; }";
        std::shared_ptr<Storage> storage = runExtractorCalls(input);

        std::string callsname = storage->getStmtToProcName(1);
        REQUIRE(callsname == "b");

        std::string callsname1 = storage->getStmtToProcName(2);
        REQUIRE(callsname1 == "c");
    }

    SECTION("Recursive Calls") {
        std::string input = "procedure a { call a; }";
        //        std::shared_ptr<Storage> storage = runExtractorCalls(input);

        // throw error
    }
}