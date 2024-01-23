#include "SPA.h"
#include "catch.hpp"
#include "sp/Parsers/Parser.h"
#include "sp/Tokenizer.h"

TEST_CASE("Testing parsing empty procedure") {
    std::string input = "procedure main { }";
    Tokenizer tokenizer(input);
    Parser parser(tokenizer.tokenize());

    REQUIRE_THROWS(parser.parse());
}

TEST_CASE("Valid: pProcedures with different name") {
    std::string input = "procedure main { y= 2;} procedure smth { y= 2; }";
    Tokenizer tokenizer(input);

    Parser parser(tokenizer.tokenize());
    const std::shared_ptr<ASTNode> result = parser.parse();

    std::shared_ptr<ASTNode> mainProc = std::make_shared<ASTNode>("proc", "main", -1);
    std::shared_ptr<ASTNode> stmtList = std::make_shared<ASTNode>("stmtList", "", -1);
    std::shared_ptr<ASTNode> assignNode = std::make_shared<ASTNode>("assign", "", 1);
    assignNode->addChild(std::make_shared<ASTNode>("var", "y", 1));
    assignNode->addChild(std::make_shared<ASTNode>("const", "2", 1));
    stmtList->addChild(assignNode);
    mainProc->addChild(stmtList);

    std::shared_ptr<ASTNode> smthProc =
        std::make_shared<ASTNode>("proc", "smth", -1);
    std::shared_ptr<ASTNode> stmtList1 =
        std::make_shared<ASTNode>("stmtList", "", -1);
    std::shared_ptr<ASTNode> assignNode1 =
        std::make_shared<ASTNode>("assign", "", 2);
    assignNode1->addChild(std::make_shared<ASTNode>("var", "y", 2));
    assignNode1->addChild(std::make_shared<ASTNode>("const", "2", 2));
    stmtList1->addChild(assignNode1);
    smthProc->addChild(stmtList1);

    const std::shared_ptr<ASTNode> expectedNode =
        std::make_shared<ASTNode>("program", "", -1);
    expectedNode->addChild(mainProc);
    expectedNode->addChild(smthProc);

    REQUIRE((*result).equals(*expectedNode));
}

TEST_CASE("Invalid procedures with same name") {
    std::string input = "procedure main { y= 2; } procedure main { y= 2; }";
    Tokenizer tokenizer(input);

    Parser parser(tokenizer.tokenize());

    REQUIRE_THROWS(parser.parse());
}

TEST_CASE("parse var_name") {
    std::string input = "procedure main { print x; }";
    Tokenizer tokenizer(input);

    Parser parser(tokenizer.tokenize());
    const std::shared_ptr<ASTNode> result = parser.parse();

    std::shared_ptr<ASTNode> expectedNode =
        std::make_shared<ASTNode>("program", "", -1);
    ASTNode mainProc("proc", "main", -1);
    ASTNode stmtList("stmtList", "", -1);

    ASTNode varNode("var", "x", 1);
    ASTNode printNode("print", "", 1);
    printNode.addChild(std::make_shared<ASTNode>(varNode));
    stmtList.addChild(std::make_shared<ASTNode>(printNode));
    mainProc.addChild(std::make_shared<ASTNode>(stmtList));
    expectedNode->addChild(std::make_shared<ASTNode>(mainProc));

    REQUIRE((*result).equals(*expectedNode));
}

TEST_CASE("parse const_value") {
    std::string input = "procedure main { x = 3; }";
    Tokenizer tokenizer(input);

    Parser parser(tokenizer.tokenize());
    ASTNode result = *(parser.parse());

    ASTNode expectedNode("program", "", -1);
    ASTNode mainProc("proc", "main", -1);
    ASTNode stmtList("stmtList", "", -1);

    ASTNode varNode("var", "x", 1);
    ASTNode constNode("const", "3", 1);
    ASTNode assignNode("assign", "", 1);
    assignNode.addChild(std::make_shared<ASTNode>(varNode));
    assignNode.addChild(std::make_shared<ASTNode>(constNode));
    stmtList.addChild(std::make_shared<ASTNode>(assignNode));
    mainProc.addChild(std::make_shared<ASTNode>(stmtList));
    expectedNode.addChild(std::make_shared<ASTNode>(mainProc));

    REQUIRE(result.equals(expectedNode));
}

TEST_CASE("parse '(' expr ')' ") {
    std::string input = "procedure main { y = (x + 2);} ";
    Tokenizer tokenizer(input);

    Parser parser(tokenizer.tokenize());
    ASTNode result = *(parser.parse());

    ASTNode expectedNode("program", "", -1);
    ASTNode mainProc("proc", "main", -1);
    ASTNode stmtList("stmtList", "", -1);

    ASTNode varNode("var", "y", 1);
    ASTNode plusNode("+", "+", 1);
    plusNode.addChild(std::make_shared<ASTNode>("var", "x", 1));
    plusNode.addChild(std::make_shared<ASTNode>("const", "2", 1));
    ASTNode assignNode("assign", "", 1);
    assignNode.addChild(std::make_shared<ASTNode>(varNode));
    assignNode.addChild(std::make_shared<ASTNode>(plusNode));
    stmtList.addChild(std::make_shared<ASTNode>(assignNode));
    mainProc.addChild(std::make_shared<ASTNode>(stmtList));
    expectedNode.addChild(std::make_shared<ASTNode>(mainProc));

    REQUIRE(result.equals(expectedNode));
}

TEST_CASE("Invalid factor ") {
    std::string input = "procedure main { y = (x + 2 ;}";
    Tokenizer tokenizer(input);

    Parser parser(tokenizer.tokenize());

    REQUIRE_THROWS(parser.parse());
}

TEST_CASE("parse 'term '*' factor' ") {
    std::string input = "procedure main { x = y * (x + 2); }";
    Tokenizer tokenizer(input);

    Parser parser(tokenizer.tokenize());
    ASTNode result = *(parser.parse());

    ASTNode expectedNode("program", "", -1);
    ASTNode mainProc("proc", "main", -1);
    ASTNode stmtList("stmtList", "", -1);

    ASTNode assignNode("assign", "", 1);
    ASTNode varNode("var", "x", 1);
    ASTNode multNode("*", "*", 1);
    multNode.addChild(std::make_shared<ASTNode>("var", "y", 1));
    ASTNode factorNode("+", "+", 1);
    factorNode.addChild(std::make_shared<ASTNode>("var", "x", 1));
    factorNode.addChild(std::make_shared<ASTNode>("const", "2", 1));
    multNode.addChild(std::make_shared<ASTNode>(factorNode));

    assignNode.addChild(std::make_shared<ASTNode>(varNode));
    assignNode.addChild(std::make_shared<ASTNode>(multNode));
    stmtList.addChild(std::make_shared<ASTNode>(assignNode));
    mainProc.addChild(std::make_shared<ASTNode>(stmtList));
    expectedNode.addChild(std::make_shared<ASTNode>(mainProc));

    REQUIRE(result.equals(expectedNode));
}

TEST_CASE("parse 'expr + term' ") {
    std::string input = "procedure main { x = (y - (x*1)) / (x + 2);} ";
    Tokenizer tokenizer(input);
    Parser parser(tokenizer.tokenize());
    ASTNode result = *(parser.parse());

    ASTNode expectedNode("program", "", -1);
    ASTNode mainProc("proc", "main", -1);
    ASTNode stmtList("stmtList", "", -1);

    ASTNode assignNode("assign", "", 1);
    ASTNode varNode("var", "x", 1);
    ASTNode rightNode("/", "/", 1);
    ASTNode rightExprNode("-", "-", 1);
    rightExprNode.addChild(std::make_shared<ASTNode>("var", "y", 1));
    ASTNode subRightNode("*", "*", 1);
    subRightNode.addChild(std::make_shared<ASTNode>("var", "x", 1));
    subRightNode.addChild(std::make_shared<ASTNode>("const", "1", 1));
    rightExprNode.addChild(std::make_shared<ASTNode>(subRightNode));

    ASTNode leftExprNode("+", "+", 1);
    leftExprNode.addChild(std::make_shared<ASTNode>("var", "x", 1));
    leftExprNode.addChild(std::make_shared<ASTNode>("const", "2", 1));

    rightNode.addChild(std::make_shared<ASTNode>(rightExprNode));
    rightNode.addChild(std::make_shared<ASTNode>(leftExprNode));

    assignNode.addChild(std::make_shared<ASTNode>(varNode));
    assignNode.addChild(std::make_shared<ASTNode>(rightNode));
    stmtList.addChild(std::make_shared<ASTNode>(assignNode));
    mainProc.addChild(std::make_shared<ASTNode>(stmtList));
    expectedNode.addChild(std::make_shared<ASTNode>(mainProc));

    REQUIRE(result.equals(expectedNode));
}

TEST_CASE("parse invalid expr 1") {
    std::string input = " procedure main { x = y (x + 2);}";
    Tokenizer tokenizer(input);
    Parser parser(tokenizer.tokenize());

    REQUIRE_THROWS(parser.parse());
}

TEST_CASE("parse invalid expr 2") {
    std::string input = " procedure main { x =3 (x + 2);}";
    Tokenizer tokenizer(input);
    Parser parser(tokenizer.tokenize());

    REQUIRE_THROWS(parser.parse());
}

TEST_CASE("parse invalid expr 3") {
    std::string input = " procedure main { x = (x + 2) y;}";
    Tokenizer tokenizer(input);
    Parser parser(tokenizer.tokenize());

    REQUIRE_THROWS(parser.parse());
}

TEST_CASE("parse invalid expr 4") {
    std::string input = " procedure main { x = (x + 2) +;} ";
    Tokenizer tokenizer(input);
    Parser parser(tokenizer.tokenize());

    REQUIRE_THROWS(parser.parse());
}

TEST_CASE("parse invalid expr 5") {
    std::string input = " procedure main { x = (x + 2) (y+2);} ";
    Tokenizer tokenizer(input);
    Parser parser(tokenizer.tokenize());

    REQUIRE_THROWS(parser.parse());
}

TEST_CASE("parse invalid expr 6") {
    std::string input = " procedure main { x =(x + 2) );}";
    Tokenizer tokenizer(input);
    Parser parser(tokenizer.tokenize());

    REQUIRE_THROWS(parser.parse());
}

TEST_CASE("parse invalid expr 7") {
    std::string input = " procedure main { x =(x + 2;} ";
    Tokenizer tokenizer(input);
    Parser parser(tokenizer.tokenize());

    REQUIRE_THROWS(parser.parse());
}

TEST_CASE("parse invalid expr 8") {
    std::string input = " procedure main { x = 3y + 2;} ";
    Tokenizer tokenizer(input);
    Parser parser(tokenizer.tokenize());

    REQUIRE_THROWS(parser.parse());
}

TEST_CASE("rel_factor '>' rel_factor") {
    std::string input = "procedure main { while(x > 2) {print x;}}";
    Tokenizer tokenizer(input);
    Parser parser(tokenizer.tokenize());
    ASTNode result = *(parser.parse());

    ASTNode expectedNode("program", "", -1);
    ASTNode mainProc("proc", "main", -1);
    ASTNode stmtList("stmtList", "", -1);

    ASTNode whileNode("while", "", 1);
    ASTNode opNode(">", ">", 1);
    opNode.addChild(std::make_shared<ASTNode>("var", "x", 1));
    opNode.addChild(std::make_shared<ASTNode>("const", "2", 1));
    whileNode.addChild(std::make_shared<ASTNode>(opNode));

    ASTNode stmtList2("stmtList", "", -1);
    ASTNode varNode("var", "x", 2);
    ASTNode printNode("print", "", 2);
    printNode.addChild(std::make_shared<ASTNode>(varNode));
    stmtList2.addChild(std::make_shared<ASTNode>(printNode));
    whileNode.addChild(std::make_shared<ASTNode>(stmtList2));

    stmtList.addChild(std::make_shared<ASTNode>(whileNode));
    mainProc.addChild(std::make_shared<ASTNode>(stmtList));
    expectedNode.addChild(std::make_shared<ASTNode>(mainProc));

    REQUIRE(result.equals(expectedNode));
}

TEST_CASE("rel_factor '!=' rel_factor") {
    std::string input = "procedure main { while(x+3 != y/2) { print x;}}";
    Tokenizer tokenizer(input);
    Parser parser(tokenizer.tokenize());
    ASTNode result = *(parser.parse());

    ASTNode notNode("!=", "!=", 1);
    ASTNode left("+", "+", 1);
    left.addChild(std::make_shared<ASTNode>("var", "x", 1));
    left.addChild(std::make_shared<ASTNode>("const", "3", 1));
    notNode.addChild(std::make_shared<ASTNode>(left));

    ASTNode right("/", "/", 1);
    right.addChild(std::make_shared<ASTNode>("var", "y", 1));
    right.addChild(std::make_shared<ASTNode>("const", "2", 1));
    notNode.addChild(std::make_shared<ASTNode>(right));

    ASTNode expectedNode("program", "", -1);
    ASTNode mainProc("proc", "main", -1);
    ASTNode stmtList("stmtList", "", -1);

    ASTNode whileNode("while", "", 1);
    whileNode.addChild(std::make_shared<ASTNode>(notNode));

    ASTNode stmtList2("stmtList", "", -1);
    ASTNode varNode("var", "x", 2);
    ASTNode printNode("print", "", 2);
    printNode.addChild(std::make_shared<ASTNode>(varNode));
    stmtList2.addChild(std::make_shared<ASTNode>(printNode));
    whileNode.addChild(std::make_shared<ASTNode>(stmtList2));

    stmtList.addChild(std::make_shared<ASTNode>(whileNode));
    mainProc.addChild(std::make_shared<ASTNode>(stmtList));
    expectedNode.addChild(std::make_shared<ASTNode>(mainProc));

    REQUIRE(result.equals(expectedNode));
}

TEST_CASE("Invalid rel_expr 1") {
    std::string input = "procedure main = { while ((x + 3 != (y / 2)) { print x;}";
    Tokenizer tokenizer(input);
    Parser parser(tokenizer.tokenize());

    REQUIRE_THROWS(parser.parse());
}

TEST_CASE("Invalid rel_expr 2") {
    std::string input = " procedure main { while (x ==) {print x;}} ";
    Tokenizer tokenizer(input);
    Parser parser(tokenizer.tokenize());

    REQUIRE_THROWS(parser.parse());
}

TEST_CASE("! (cond_expr)") {
    std::string input = "procedure main { while (!(x > 2)) { print x;}}";
    Tokenizer tokenizer(input);
    Parser parser(tokenizer.tokenize());
    ASTNode result = *(parser.parse());

    ASTNode notNode("!", "!", 1);
    ASTNode subNode(">", ">", 1);
    subNode.addChild(std::make_shared<ASTNode>("var", "x", 1));
    subNode.addChild(std::make_shared<ASTNode>("const", "2", 1));
    notNode.addChild(std::make_shared<ASTNode>(subNode));

    ASTNode expectedNode("program", "", -1);
    ASTNode mainProc("proc", "main", -1);
    ASTNode stmtList("stmtList", "", -1);

    ASTNode whileNode("while", "", 1);
    whileNode.addChild(std::make_shared<ASTNode>(notNode));

    ASTNode stmtList2("stmtList", "", -1);
    ASTNode varNode("var", "x", 2);
    ASTNode printNode("print", "", 2);
    printNode.addChild(std::make_shared<ASTNode>(varNode));
    stmtList2.addChild(std::make_shared<ASTNode>(printNode));
    whileNode.addChild(std::make_shared<ASTNode>(stmtList2));

    stmtList.addChild(std::make_shared<ASTNode>(whileNode));
    mainProc.addChild(std::make_shared<ASTNode>(stmtList));
    expectedNode.addChild(std::make_shared<ASTNode>(mainProc));

    REQUIRE(result.equals(expectedNode));
}

TEST_CASE(" (cond_expr) && (cond_expr)") {
    std::string input = "procedure main { while ((x > 2) && ((y == 1) && (z<=3))) {print x;}}";
    Tokenizer tokenizer(input);
    Parser parser(tokenizer.tokenize());
    ASTNode result = *(parser.parse());

    ASTNode condNode("&&", "&&", 1);
    ASTNode subNode(">", ">", 1);
    subNode.addChild(std::make_shared<ASTNode>("var", "x", 1));
    subNode.addChild(std::make_shared<ASTNode>("const", "2", 1));

    ASTNode subNode2("==", "==", 1);
    subNode2.addChild(std::make_shared<ASTNode>("var", "y", 1));
    subNode2.addChild(std::make_shared<ASTNode>("const", "1", 1));

    ASTNode subNode3("<=", "<=", 1);
    subNode3.addChild(std::make_shared<ASTNode>("var", "z", 1));
    subNode3.addChild(std::make_shared<ASTNode>("const", "3", 1));

    ASTNode and1("&&", "&&", 1);
    and1.addChild(std::make_shared<ASTNode>(subNode2));
    and1.addChild(std::make_shared<ASTNode>(subNode3));
    condNode.addChild(std::make_shared<ASTNode>(subNode));
    condNode.addChild(std::make_shared<ASTNode>(and1));

    ASTNode expectedNode("program", "", -1);
    ASTNode mainProc("proc", "main", -1);
    ASTNode stmtList("stmtList", "", -1);

    ASTNode whileNode("while", "", 1);
    whileNode.addChild(std::make_shared<ASTNode>(condNode));

    ASTNode stmtList2("stmtList", "", -1);
    ASTNode varNode("var", "x", 2);
    ASTNode printNode("print", "", 2);
    printNode.addChild(std::make_shared<ASTNode>(varNode));
    stmtList2.addChild(std::make_shared<ASTNode>(printNode));
    whileNode.addChild(std::make_shared<ASTNode>(stmtList2));

    stmtList.addChild(std::make_shared<ASTNode>(whileNode));
    mainProc.addChild(std::make_shared<ASTNode>(stmtList));
    expectedNode.addChild(std::make_shared<ASTNode>(mainProc));

    REQUIRE(result.equals(expectedNode));
}

TEST_CASE(" Valid assign stmt with simple expr") {
    std::string input = " procedure main { x = 42; }";
    Tokenizer tokenizer(input);
    Parser parser(tokenizer.tokenize());
    ASTNode result = *(parser.parse());

    ASTNode assignNode("assign", "", 1);
    assignNode.addChild(std::make_shared<ASTNode>("var", "x", 1));
    assignNode.addChild(std::make_shared<ASTNode>("const", "42", 1));

    ASTNode expectedNode("program", "", -1);
    ASTNode mainProc("proc", "main", -1);
    ASTNode stmtList("stmtList", "", -1);

    stmtList.addChild(std::make_shared<ASTNode>(assignNode));
    mainProc.addChild(std::make_shared<ASTNode>(stmtList));
    expectedNode.addChild(std::make_shared<ASTNode>(mainProc));

    REQUIRE(result.equals(expectedNode));
}

TEST_CASE(" Valid assign stmt with complex expr") {
    std::string input = " procedure main { x = y + 3 / 2; }";
    Tokenizer tokenizer(input);
    Parser parser(tokenizer.tokenize());
    ASTNode result = *(parser.parse());

    ASTNode assignNode("assign", "", 1);
    assignNode.addChild(std::make_shared<ASTNode>("var", "x", 1));
    ASTNode exprNode("+", "+", 1);
    ASTNode divNode("/", "/", 1);
    divNode.addChild(std::make_shared<ASTNode>("const", "3", 1));
    divNode.addChild(std::make_shared<ASTNode>("const", "2", 1));
    exprNode.addChild(std::make_shared<ASTNode>("var", "y", 1));
    exprNode.addChild(std::make_shared<ASTNode>(divNode));
    assignNode.addChild(std::make_shared<ASTNode>(exprNode));

    ASTNode expectedNode("program", "", -1);
    ASTNode mainProc("proc", "main", -1);
    ASTNode stmtList("stmtList", "", -1);

    stmtList.addChild(std::make_shared<ASTNode>(assignNode));
    mainProc.addChild(std::make_shared<ASTNode>(stmtList));
    expectedNode.addChild(std::make_shared<ASTNode>(mainProc));

    REQUIRE(result.equals(expectedNode));
}

TEST_CASE(" Invalid assign stmt missing semicolon") {
    std::string input = " procedure main { x = y + 3 }";
    Tokenizer tokenizer(input);
    Parser parser(tokenizer.tokenize());

    REQUIRE_THROWS(parser.parse());
}

TEST_CASE("Valid read stmt") {
    Tokenizer tokenizer("procedure main { read y;}");
    Parser parser(tokenizer.tokenize());
    ASTNode result = *(parser.parse());

    ASTNode readNode("read", "", 1);
    readNode.addChild(std::make_shared<ASTNode>("var", "y", 1));

    ASTNode expectedNode("program", "", -1);
    ASTNode mainProc("proc", "main", -1);
    ASTNode stmtList("stmtList", "", -1);

    stmtList.addChild(std::make_shared<ASTNode>(readNode));
    mainProc.addChild(std::make_shared<ASTNode>(stmtList));
    expectedNode.addChild(std::make_shared<ASTNode>(mainProc));

    REQUIRE(result.equals(expectedNode));
}

TEST_CASE("Invalid read stmt, cannot read numbers") {
    Tokenizer tokenizer("procedure main { read 0; }");
    Parser parser(tokenizer.tokenize());

    REQUIRE_THROWS(parser.parse());
}

TEST_CASE("Valid print stmt") {
    Tokenizer tokenizer("procedure main { print y;}");
    Parser parser(tokenizer.tokenize());
    ASTNode result = *(parser.parse());

    ASTNode printNode("print", "", 1);
    printNode.addChild(std::make_shared<ASTNode>("var", "y", 1));

    ASTNode expectedNode("program", "", -1);
    ASTNode mainProc("proc", "main", -1);
    ASTNode stmtList("stmtList", "", -1);

    stmtList.addChild(std::make_shared<ASTNode>(printNode));
    mainProc.addChild(std::make_shared<ASTNode>(stmtList));
    expectedNode.addChild(std::make_shared<ASTNode>(mainProc));

    REQUIRE(result.equals(expectedNode));
}

TEST_CASE("Invalid print stmt, cannot read numbers") {
    Tokenizer tokenizer("procedure main { print 0; }");
    Parser parser(tokenizer.tokenize());

    REQUIRE_THROWS(parser.parse());
}

TEST_CASE("Valid simple if statement") {
    Tokenizer tokenizer("procedure main { if (y < 2) then { read y; } else { print y; }}");
    Parser parser(tokenizer.tokenize());
    ASTNode result = *(parser.parse());

    ASTNode ifNode("if", "", 1);
    ASTNode condNode("<", "<", 1);
    condNode.addChild(std::make_shared<ASTNode>("var", "y", 1));
    condNode.addChild(std::make_shared<ASTNode>("const", "2", 1));
    ifNode.addChild(std::make_shared<ASTNode>(condNode));
    ASTNode thenStmtListNode("stmtList", "then", -1);
    ASTNode readNode("read", "", 2);
    readNode.addChild(std::make_shared<ASTNode>("var", "y", 2));
    thenStmtListNode.addChild(std::make_shared<ASTNode>(readNode));
    ifNode.addChild(std::make_shared<ASTNode>(thenStmtListNode));

    ASTNode elseStmtListNode("stmtList", "else", -1);
    ASTNode printNode("print", "", 3);
    printNode.addChild(std::make_shared<ASTNode>("var", "y", 3));
    elseStmtListNode.addChild(std::make_shared<ASTNode>(printNode));
    ifNode.addChild(std::make_shared<ASTNode>(elseStmtListNode));

    ASTNode expectedNode("program", "", -1);
    ASTNode mainProc("proc", "main", -1);
    ASTNode stmtList("stmtList", "", -1);

    stmtList.addChild(std::make_shared<ASTNode>(ifNode));
    mainProc.addChild(std::make_shared<ASTNode>(stmtList));
    expectedNode.addChild(std::make_shared<ASTNode>(mainProc));

    REQUIRE(result.equals(expectedNode));
}

TEST_CASE("Valid if statement with complex cond expr") {
    Tokenizer tokenizer("procedure main { if (x+3 == y-2) then { read y; } else { print y; }}");
    Parser parser(tokenizer.tokenize());
    ASTNode result = *(parser.parse());

    ASTNode ifNode("if", "", 1);
    ASTNode condNode("==", "==", 1);
    ASTNode leftCond("+", "+", 1);
    leftCond.addChild(std::make_shared<ASTNode>("var", "x", 1));
    leftCond.addChild(std::make_shared<ASTNode>("const", "3", 1));
    ASTNode rightCond("-", "-", 1);
    rightCond.addChild(std::make_shared<ASTNode>("var", "y", 1));
    rightCond.addChild(std::make_shared<ASTNode>("const", "2", 1));
    condNode.addChild(std::make_shared<ASTNode>(leftCond));
    condNode.addChild(std::make_shared<ASTNode>(rightCond));

    ifNode.addChild(std::make_shared<ASTNode>(condNode));
    ASTNode thenStmtListNode("stmtList", "then", -1);
    ASTNode readNode("read", "", 2);
    readNode.addChild(std::make_shared<ASTNode>("var", "y", 2));
    thenStmtListNode.addChild(std::make_shared<ASTNode>(readNode));
    ifNode.addChild(std::make_shared<ASTNode>(thenStmtListNode));

    ASTNode elseStmtListNode("stmtList", "else", -1);
    ASTNode printNode("print", "", 3);
    printNode.addChild(std::make_shared<ASTNode>("var", "y", 3));
    elseStmtListNode.addChild(std::make_shared<ASTNode>(printNode));
    ifNode.addChild(std::make_shared<ASTNode>(elseStmtListNode));

    ASTNode expectedNode("program", "", -1);
    ASTNode mainProc("proc", "main", -1);
    ASTNode stmtList("stmtList", "", -1);

    stmtList.addChild(std::make_shared<ASTNode>(ifNode));
    mainProc.addChild(std::make_shared<ASTNode>(stmtList));
    expectedNode.addChild(std::make_shared<ASTNode>(mainProc));

    REQUIRE(result.equals(expectedNode));
}

TEST_CASE("Valid if statements with multiple nested statements") {
    Tokenizer tokenizer("procedure main { if (y < 2) then { x = 1; read y; } else { print y; }} ");
    Parser parser(tokenizer.tokenize());
    ASTNode result = *(parser.parse());

    ASTNode ifNode("if", "", 1);
    ASTNode condNode("<", "<", 1);
    condNode.addChild(std::make_shared<ASTNode>("var", "y", 1));
    condNode.addChild(std::make_shared<ASTNode>("const", "2", 1));
    ifNode.addChild(std::make_shared<ASTNode>(condNode));
    ASTNode thenStmtListNode("stmtList", "then", -1);
    ASTNode assignNode("assign", "", 2);
    assignNode.addChild(std::make_shared<ASTNode>("var", "x", 2));
    assignNode.addChild(std::make_shared<ASTNode>("const", "1", 2));
    ASTNode readNode("read", "", 3);
    readNode.addChild(std::make_shared<ASTNode>("var", "y", 3));
    thenStmtListNode.addChild(std::make_shared<ASTNode>(assignNode));
    thenStmtListNode.addChild(std::make_shared<ASTNode>(readNode));
    ifNode.addChild(std::make_shared<ASTNode>(thenStmtListNode));

    ASTNode elseStmtListNode("stmtList", "else", -1);
    ASTNode printNode("print", "", 4);
    printNode.addChild(std::make_shared<ASTNode>("var", "y", 4));
    elseStmtListNode.addChild(std::make_shared<ASTNode>(printNode));
    ifNode.addChild(std::make_shared<ASTNode>(elseStmtListNode));

    ASTNode expectedNode("program", "", -1);
    ASTNode mainProc("proc", "main", -1);
    ASTNode stmtList("stmtList", "", -1);

    stmtList.addChild(std::make_shared<ASTNode>(ifNode));
    mainProc.addChild(std::make_shared<ASTNode>(stmtList));
    expectedNode.addChild(std::make_shared<ASTNode>(mainProc));

    REQUIRE(result.equals(expectedNode));
}

TEST_CASE("Valid if statements with nested if statement") {
    Tokenizer tokenizer("procedure main { if (y < 2) then { if (x == 4) then {print y;} else { read x;} } else { print y; }}");
    Parser parser(tokenizer.tokenize());
    ASTNode result = *(parser.parse());

    ASTNode subIfNode("if", "", 2);
    ASTNode condNode1("==", "==", 2);
    condNode1.addChild(std::make_shared<ASTNode>("var", "x", 2));
    condNode1.addChild(std::make_shared<ASTNode>("const", "4", 2));
    subIfNode.addChild(std::make_shared<ASTNode>(condNode1));
    ASTNode thenStmtListNode1("stmtList", "then", -1);
    ASTNode printNode1("print", "", 3);
    printNode1.addChild(std::make_shared<ASTNode>("var", "y", 3));
    thenStmtListNode1.addChild(std::make_shared<ASTNode>(printNode1));
    subIfNode.addChild(std::make_shared<ASTNode>(thenStmtListNode1));
    ASTNode elseStmtListNode1("stmtList", "else", -1);
    ASTNode readNode1("read", "", 4);
    readNode1.addChild(std::make_shared<ASTNode>("var", "x", 4));
    elseStmtListNode1.addChild(std::make_shared<ASTNode>(readNode1));
    subIfNode.addChild(std::make_shared<ASTNode>(elseStmtListNode1));

    ASTNode ifNode("if", "", 1);
    ASTNode condNode("<", "<", 1);
    condNode.addChild(std::make_shared<ASTNode>("var", "y", 1));
    condNode.addChild(std::make_shared<ASTNode>("const", "2", 1));
    ifNode.addChild(std::make_shared<ASTNode>(condNode));
    ASTNode thenStmtListNode("stmtList", "then", -1);
    thenStmtListNode.addChild(std::make_shared<ASTNode>(subIfNode));
    ifNode.addChild(std::make_shared<ASTNode>(thenStmtListNode));

    ASTNode elseStmtListNode("stmtList", "else", -1);
    ASTNode printNode("print", "", 5);
    printNode.addChild(std::make_shared<ASTNode>("var", "y", 5));
    elseStmtListNode.addChild(std::make_shared<ASTNode>(printNode));
    ifNode.addChild(std::make_shared<ASTNode>(elseStmtListNode));

    ASTNode expectedNode("program", "", -1);
    ASTNode mainProc("proc", "main", -1);
    ASTNode stmtList("stmtList", "", -1);

    stmtList.addChild(std::make_shared<ASTNode>(ifNode));
    mainProc.addChild(std::make_shared<ASTNode>(stmtList));
    expectedNode.addChild(std::make_shared<ASTNode>(mainProc));

    REQUIRE(result.equals(expectedNode));
}

TEST_CASE("Invalid if stmt missing then") {
    Tokenizer tokenizer("procedure main { if (x+3 == y-2) { read y; } else { print y; } } ");
    Parser parser(tokenizer.tokenize());
    REQUIRE_THROWS(parser.parse());
}

TEST_CASE("Invalid if stmt additonal token after if") {
    Tokenizer tokenizer("procedure main { if smth(x+3 == y-2) then { read y; } else { print y; }}");
    Parser parser(tokenizer.tokenize());
    REQUIRE_THROWS(parser.parse());
}

TEST_CASE("Invalid if stmt missing else ") {
    Tokenizer tokenizer("procedure main {if (x+3 == y-2) then { read y; }  { print y; }}");
    Parser parser(tokenizer.tokenize());
    REQUIRE_THROWS(parser.parse());
}

TEST_CASE("Invalid if stmt close brackets ") {
    Tokenizer tokenizer("procedure main {if (x+3 == y-2) then { read y;  else { print y; }}");
    Parser parser(tokenizer.tokenize());
    REQUIRE_THROWS(parser.parse());
}

TEST_CASE("Valid simple while statement") {
    Tokenizer tokenizer("procedure main { while (y < 2) { print y;} } ");
    Parser parser(tokenizer.tokenize());
    ASTNode result = *(parser.parse());

    ASTNode whileNode("while", "", 1);
    ASTNode condNode("<", "<", 1);
    condNode.addChild(std::make_shared<ASTNode>("var", "y", 1));
    condNode.addChild(std::make_shared<ASTNode>("const", "2", 1));
    whileNode.addChild(std::make_shared<ASTNode>(condNode));

    ASTNode whileStmtListNode("stmtList", "", -1);
    ASTNode printNode("print", "", 2);
    printNode.addChild(std::make_shared<ASTNode>("var", "y", 2));
    whileStmtListNode.addChild(std::make_shared<ASTNode>(printNode));
    whileNode.addChild(std::make_shared<ASTNode>(whileStmtListNode));

    ASTNode expectedNode("program", "", -1);
    ASTNode mainProc("proc", "main", -1);
    ASTNode stmtList("stmtList", "", -1);

    stmtList.addChild(std::make_shared<ASTNode>(whileNode));
    mainProc.addChild(std::make_shared<ASTNode>(stmtList));
    expectedNode.addChild(std::make_shared<ASTNode>(mainProc));

    REQUIRE(result.equals(expectedNode));
}

TEST_CASE("Valid complex cond expr while statement") {
    Tokenizer tokenizer("procedure main { while (!((y == 1) && (z<=3))) { print y; y = x + 2; }}");
    Parser parser(tokenizer.tokenize());
    ASTNode result = *(parser.parse());

    ASTNode whileNode("while", "", 1);
    ASTNode subNode("==", "==", 1);
    subNode.addChild(std::make_shared<ASTNode>("var", "y", 1));
    subNode.addChild(std::make_shared<ASTNode>("const", "1", 1));

    ASTNode subNode2("<=", "<=", 1);
    subNode2.addChild(std::make_shared<ASTNode>("var", "z", 1));
    subNode2.addChild(std::make_shared<ASTNode>("const", "3", 1));

    ASTNode andNode("&&", "&&", 1);
    andNode.addChild(std::make_shared<ASTNode>(subNode));
    andNode.addChild(std::make_shared<ASTNode>(subNode2));

    ASTNode notNode("!", "!", 1);
    notNode.addChild(std::make_shared<ASTNode>(andNode));

    ASTNode whileStmtListNode("stmtList", "", -1);
    ASTNode printNode("print", "", 2);
    printNode.addChild(std::make_shared<ASTNode>("var", "y", 2));
    ASTNode assignNode("assign", "", 3);
    assignNode.addChild(std::make_shared<ASTNode>("var", "y", 3));
    ASTNode plusNode("+", "+", 3);
    plusNode.addChild(std::make_shared<ASTNode>("var", "x", 3));
    plusNode.addChild(std::make_shared<ASTNode>("const", "2", 3));
    assignNode.addChild(std::make_shared<ASTNode>(plusNode));
    whileStmtListNode.addChild(std::make_shared<ASTNode>(printNode));
    whileStmtListNode.addChild(std::make_shared<ASTNode>(assignNode));
    whileNode.addChild(std::make_shared<ASTNode>(notNode));
    whileNode.addChild(std::make_shared<ASTNode>(whileStmtListNode));

    ASTNode expectedNode("program", "", -1);
    ASTNode mainProc("proc", "main", -1);
    ASTNode stmtList("stmtList", "", -1);

    stmtList.addChild(std::make_shared<ASTNode>(whileNode));
    mainProc.addChild(std::make_shared<ASTNode>(stmtList));
    expectedNode.addChild(std::make_shared<ASTNode>(mainProc));

    REQUIRE(result.equals(expectedNode));
}

TEST_CASE("Invalid empty while statement") {
    Tokenizer tokenizer("procedure main { while (y < 2) { }}");
    Parser parser(tokenizer.tokenize());

    REQUIRE_THROWS(parser.parse());
}

TEST_CASE("Invalid empty cond expr while statement") {
    Tokenizer tokenizer("procedure main { while () { print y; }}");
    Parser parser(tokenizer.tokenize());

    REQUIRE_THROWS(parser.parse());
}

TEST_CASE("Invalid while stmt missing {") {
    Tokenizer tokenizer("procedure main { while (y < 2)  print y; }}");
    Parser parser(tokenizer.tokenize());
    REQUIRE_THROWS(parser.parse());
}

TEST_CASE("Invalid whiletmt additional )") {
    Tokenizer tokenizer("procedure main {while (y < 2)) { print y; }}");
    Parser parser(tokenizer.tokenize());
    REQUIRE_THROWS(parser.parse());
}

TEST_CASE("Invalid while stmt condr expr ") {
    Tokenizer tokenizer("procedure main {while (y < 2 + ) { print y; }}");
    Parser parser(tokenizer.tokenize());
    REQUIRE_THROWS(parser.parse());
}

TEST_CASE("Test parsing multiple statements") {
    std::string input = "procedure main { read y; x = 42; y =3; if (y < 2) then { read y; } else { print y; } print x;}";
    Tokenizer tokenizer(input);
    Parser parser(tokenizer.tokenize());
    ASTNode result = *(parser.parse());

    ASTNode listNode("stmtList", "", -1);
    ASTNode readNode("read", "", 1);
    readNode.addChild(std::make_shared<ASTNode>("var", "y", 1));
    ASTNode assignNode("assign", "", 2);
    assignNode.addChild(std::make_shared<ASTNode>("var", "x", 2));
    assignNode.addChild(std::make_shared<ASTNode>("const", "42", 2));
    ASTNode assignNode2("assign", "", 3);
    assignNode2.addChild(std::make_shared<ASTNode>("var", "y", 3));
    assignNode2.addChild(std::make_shared<ASTNode>("const", "3", 3));

    ASTNode ifNode("if", "", 4);
    ASTNode condNode("<", "<", 4);
    condNode.addChild(std::make_shared<ASTNode>("var", "y", 4));
    condNode.addChild(std::make_shared<ASTNode>("const", "2", 4));
    ifNode.addChild(std::make_shared<ASTNode>(condNode));
    ASTNode thenStmtListNode("stmtList", "then", -1);
    ASTNode readNode2("read", "", 5);
    readNode2.addChild(std::make_shared<ASTNode>("var", "y", 5));
    thenStmtListNode.addChild(std::make_shared<ASTNode>(readNode2));
    ifNode.addChild(std::make_shared<ASTNode>(thenStmtListNode));

    ASTNode elseStmtListNode("stmtList", "else", -1);
    ASTNode printNode("print", "", 6);
    printNode.addChild(std::make_shared<ASTNode>("var", "y", 6));
    elseStmtListNode.addChild(std::make_shared<ASTNode>(printNode));
    ifNode.addChild(std::make_shared<ASTNode>(elseStmtListNode));

    ASTNode printNode2("print", "", 7);
    printNode2.addChild(std::make_shared<ASTNode>("var", "x", 7));

    listNode.addChild(std::make_shared<ASTNode>(readNode));
    listNode.addChild(std::make_shared<ASTNode>(assignNode));
    listNode.addChild(std::make_shared<ASTNode>(assignNode2));
    listNode.addChild(std::make_shared<ASTNode>(ifNode));
    listNode.addChild(std::make_shared<ASTNode>(printNode2));

    ASTNode expectedNode("program", "", -1);
    ASTNode mainProc("proc", "main", -1);

    mainProc.addChild(std::make_shared<ASTNode>(listNode));
    expectedNode.addChild(std::make_shared<ASTNode>(mainProc));

    REQUIRE(result.equals(expectedNode));
}
