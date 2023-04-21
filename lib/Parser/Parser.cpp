#include "chilang/Parser/Parser.h"

std::unique_ptr<AST_BaseNode> Parser::Parse(){
    std::unique_ptr<AST_BaseNode> tree = std::move(ParseStmt());
    // IsNextTokenOfType(Token::kEOI);
    return tree;
}

std::unique_ptr<AST_BaseNode> Parser::ParseStmt(){
    return ParseExprStmt();
}

// 解析语句
// stmt = exprStmt
std::unique_ptr<AST_BaseNode> Parser::ParseExprStmt(){
    std::unique_ptr<AST_BaseNode> node = std::make_unique<AST_newBinaryNode>(AST_BaseNode::ND_EXPR_STMT, std::move(ParseExpr()), nullptr);
    Advance(";");
    return node;
}

// 解析表达式语句
// exprStmt = expr ";"
std::unique_ptr<AST_BaseNode> Parser::ParseExpr(){
    return ParseEquality();
}

// 解析相等性
// equality = relational ("==" relational | "!=" relational)*
std::unique_ptr<AST_BaseNode>  Parser::ParseEquality(){
    // relational
    std::unique_ptr<AST_BaseNode> left = std::move(ParseRelational());
    // ("==" relational | "!=" relational)*
    while(token.IsPUNCT()){
        if(token.Is(Token::TK_EQ)){
            Advance();
            std::unique_ptr<AST_BaseNode> right = std::move(ParseRelational());
            left = std::move(std::make_unique<AST_newBinaryNode>(AST_BaseNode::ND_EQ, std::move(left), std::move(right)));
        }
        else if(token.Is(Token::TK_NE)){
            Advance();
            std::unique_ptr<AST_BaseNode> right = std::move(ParseRelational());
            left = std::move(std::make_unique<AST_newBinaryNode>(AST_BaseNode::ND_NE, std::move(left), std::move(right)));
        }
        else{
            break;
        }
    }
    return left;
}

// 解析比较关系
// relational = add ("<" add | "<=" add | ">" add | ">=" add)*
std::unique_ptr<AST_BaseNode> Parser::ParseRelational(){
    // add
    std::unique_ptr<AST_BaseNode> left = std::move(ParseAdd());
    // ("<" add | "<=" add | ">" add | ">=" add)*
    while(token.IsPUNCT()){
        if(token.Is(Token::TK_LT)){
            Advance();
            std::unique_ptr<AST_BaseNode> right = std::move(ParseAdd());
            left = std::move(std::make_unique<AST_newBinaryNode>(AST_BaseNode::ND_LT, std::move(left), std::move(right)));
        }
        else if(token.Is(Token::TK_GT)){
            Advance();
            std::unique_ptr<AST_BaseNode> right = ParseAdd();
            left = std::move(std::make_unique<AST_newBinaryNode>(AST_BaseNode::ND_GT, std::move(left), std::move(right)));
        }
        else if(token.Is(Token::TK_LE)){
            Advance();
            std::unique_ptr<AST_BaseNode> right = ParseAdd();
            left = std::move(std::make_unique<AST_newBinaryNode>(AST_BaseNode::ND_LE, std::move(left), std::move(right)));
        }
        else if(token.Is(Token::TK_GE)){
            Advance();
            std::unique_ptr<AST_BaseNode> right = ParseAdd();
            left = std::move(std::make_unique<AST_newBinaryNode>(AST_BaseNode::ND_GE, std::move(left), std::move(right)));
        }
        else{
            break;
        }
    }
    return left;
}

// 解析加减
// add = mul ("+" mul | "-" mul)*
std::unique_ptr<AST_BaseNode> Parser::ParseAdd(){
    std::unique_ptr<AST_BaseNode> left = std::move(ParseMul());
    while(token.IsPUNCT()){
        if(token.Is(Token::TK_PLUS)){
            Advance();
            std::unique_ptr<AST_BaseNode> right = std::move(ParseMul());
            left = std::move(std::make_unique<AST_newBinaryNode>(AST_BaseNode::ND_ADD, std::move(left), std::move(right)));
        }
        else if(token.Is(Token::TK_MINUS)){
            Advance();
            std::unique_ptr<AST_BaseNode> right = std::move(ParseMul());
            left = std::move(std::make_unique<AST_newBinaryNode>(AST_BaseNode::ND_SUB, std::move(left), std::move(right)));
        }
        else{
            break;
        }
    }
    return left;
}

// 解析乘除
// mul = unary ("*" unary | "/" unary)*
std::unique_ptr<AST_BaseNode> Parser::ParseMul(){
    // 解析一元运算
    std::unique_ptr<AST_BaseNode> left = std::move(ParseUnary());
    while(token.IsPUNCT()){
        if(token.Is(Token::TK_MUL)){
            Advance();
            std::unique_ptr<AST_BaseNode> right = std::move(ParseUnary());
            left = std::move(std::make_unique<AST_newBinaryNode>(AST_BaseNode::ND_MUL, std::move(left), std::move(right)));
        }
        else if(token.Is(Token::TK_DIV)){
            Advance();
            std::unique_ptr<AST_BaseNode> right = std::move(ParseUnary());
            left = std::move(std::make_unique<AST_newBinaryNode>(AST_BaseNode::ND_DIV, std::move(left), std::move(right)));
        }
        else{
            break;
        }
    }
    return left;
}

//解析一元运算符
std::unique_ptr<AST_BaseNode> Parser::ParseUnary(){
    if(token.Is(Token::TK_PLUS)){
        Advance();
        std::unique_ptr<AST_BaseNode> left = std::move(ParseUnary());
        return std::make_unique<AST_newBinaryNode>(AST_BaseNode::ND_POS, std::move(left), nullptr);
    }
    if(token.Is(Token::TK_MINUS)){
        Advance();
        std::unique_ptr<AST_BaseNode> left = std::move(ParseUnary());
        return std::make_unique<AST_newBinaryNode>(AST_BaseNode::ND_NEG, std::move(left), nullptr);
    }
    std::unique_ptr<AST_BaseNode> left = std::move(ParsePrimary());
    return left;
}

// 解析括号、数字
// primary = "(" expr ")" | num
std::unique_ptr<AST_BaseNode> Parser::ParsePrimary(){
    std::unique_ptr<AST_BaseNode> left = nullptr;
    if(token.Is(Token::TK_LEFTPAREN)){
        Advance("(");
        left = std::move(ParseExpr());
        Advance(")");
    }
    if(token.Is(Token::TK_NUM)){
        left = std::move(std::make_unique<AST_newNumNode>(AST_BaseNode::ND_NUM, token.GetValue()));
        Advance();
    }
    return left;
}