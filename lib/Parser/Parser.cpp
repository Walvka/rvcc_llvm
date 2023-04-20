#include "chilang/Parser/Parser.h"

AST_BaseNode* Parser::Parse(){
    AST_BaseNode* tree = ParseStmt();
    // IsNextTokenOfType(Token::kEOI);
    return tree;
}

AST_BaseNode* Parser::ParseStmt(){
    return ParseExprStmt();
}

// 解析语句
// stmt = exprStmt
AST_BaseNode* Parser::ParseExprStmt(){
    AST_BaseNode* node = new AST_newBinaryNode(AST_BaseNode::ND_EXPR_STMT, ParseExpr(), NULL);
    Advance(";");
    return node;
}

// 解析表达式语句
// exprStmt = expr ";"
AST_BaseNode* Parser::ParseExpr(){
    return ParseEquality();
}

// 解析相等性
// equality = relational ("==" relational | "!=" relational)*
AST_BaseNode*  Parser::ParseEquality(){
    // relational
    AST_BaseNode* left = ParseRelational();
    // ("==" relational | "!=" relational)*
    while(token.IsPUNCT()){
        if(token.Is(Token::TK_EQ)){
            Advance();
            AST_BaseNode* right = ParseRelational();
            left = new AST_newBinaryNode(AST_BaseNode::ND_EQ, left, right);
        }
        else if(token.Is(Token::TK_NE)){
            Advance();
            AST_BaseNode* right = ParseRelational();
            left = new AST_newBinaryNode(AST_BaseNode::ND_NE, left, right);
        }
        else{
            break;
        }
    }
    return left;
}

// 解析比较关系
// relational = add ("<" add | "<=" add | ">" add | ">=" add)*
AST_BaseNode*   Parser::ParseRelational(){
    // add
    AST_BaseNode* left = ParseAdd();
    // ("<" add | "<=" add | ">" add | ">=" add)*
    while(token.IsPUNCT()){
        if(token.Is(Token::TK_LT)){
            Advance();
            AST_BaseNode* right = ParseAdd();
            left = new AST_newBinaryNode(AST_BaseNode::ND_LT, left, right);
        }
        else if(token.Is(Token::TK_GT)){
            Advance();
            AST_BaseNode* right = ParseAdd();
            left = new AST_newBinaryNode(AST_BaseNode::ND_GT, left, right);
        }
        else if(token.Is(Token::TK_LE)){
            Advance();
            AST_BaseNode* right = ParseAdd();
            left = new AST_newBinaryNode(AST_BaseNode::ND_LE, left, right);
        }
        else if(token.Is(Token::TK_GE)){
            Advance();
            AST_BaseNode* right = ParseAdd();
            left = new AST_newBinaryNode(AST_BaseNode::ND_GE, left, right);
        }
        else{
            break;
        }
    }
    return left;
}

// 解析加减
// add = mul ("+" mul | "-" mul)*
AST_BaseNode* Parser::ParseAdd(){
    AST_BaseNode* left = ParseMul();
    while(token.IsPUNCT()){
        if(token.Is(Token::TK_PLUS)){
            Advance();
            AST_BaseNode* right = ParseMul();
            left = new AST_newBinaryNode(AST_BaseNode::ND_ADD, left, right);
        }
        else if(token.Is(Token::TK_MINUS)){
            Advance();
            AST_BaseNode* right = ParseMul();
            left = new AST_newBinaryNode(AST_BaseNode::ND_SUB, left, right);
        }
        else{
            break;
        }
    }
    return left;
}

// 解析乘除
// mul = unary ("*" unary | "/" unary)*
AST_BaseNode* Parser::ParseMul(){
    // 解析一元运算
    AST_BaseNode* left = ParseUnary();
    while(token.IsPUNCT()){
        if(token.Is(Token::TK_MUL)){
            Advance();
            AST_BaseNode* right = ParseUnary();
            left = new AST_newBinaryNode(AST_BaseNode::ND_MUL, left, right);
        }
        else if(token.Is(Token::TK_DIV)){
            Advance();
            AST_BaseNode* right = ParseUnary();
            left = new AST_newBinaryNode(AST_BaseNode::ND_DIV, left, right);
        }
        else{
            break;
        }
    }
    return left;
}

//解析一元运算符
AST_BaseNode* Parser::ParseUnary(){
    AST_BaseNode* left = NULL;
    if(token.Is(Token::TK_PLUS)){
        Advance();
        left = ParseUnary();
        return new AST_newBinaryNode(AST_BaseNode::ND_POS, left, NULL);
    }
    if(token.Is(Token::TK_MINUS)){
        Advance();
        left = ParseUnary();
        return new AST_newBinaryNode(AST_BaseNode::ND_NEG, left, NULL);
    }
    left = ParsePrimary();
    return left;
}

// 解析括号、数字
// primary = "(" expr ")" | num
AST_BaseNode* Parser::ParsePrimary(){
    AST_BaseNode* left = NULL;
    if(token.Is(Token::TK_LEFTPAREN)){
        Advance("(");
        left = ParseExpr();
        Advance(")");
    }
    if(token.Is(Token::TK_NUM)){
        left = new AST_newNumNode(AST_BaseNode::ND_NUM, token.GetValue());
        Advance();
    }
    return left;
}