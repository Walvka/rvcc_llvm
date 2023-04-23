#pragma once

#include "chilang/Parser/AST.h"
#include "chilang/Lexer/Lexer.h"

#include "llvm/Support/raw_ostream.h"

// program = stmt*
// stmt = exprStmt
// exprStmt = expr ";"
// expr = assign
// assign = equality ("=" assign)?
// equality = relational ("==" relational | "!=" relational)*
// relational = add ("<" add | "<=" add | ">" add | ">=" add)*
// add = mul ("+" mul | "-" mul)*
// mul = unary ("*" unary | "/" unary)*
// unary = ("+" | "-") unary | primary
// primary = "(" expr ")" | num
class Parser{
public:
    Parser(Lexer& lexer) : lexer(lexer),hasError(false){
        Advance();
    }

    void Parse();

    std::unique_ptr<AST_BaseNode> GetParseTree();
    bool ParseTreeIsEnd();
    
    bool HasError(){
        return hasError;
    }

private:
    std::unique_ptr<AST_BaseNode>   ParseStmt();
    std::unique_ptr<AST_BaseNode>   ParseExprStmt();
    std::unique_ptr<AST_BaseNode>   ParseExpr();
    std::unique_ptr<AST_BaseNode>   ParseAssign();
    std::unique_ptr<AST_BaseNode>   ParseEquality();
    std::unique_ptr<AST_BaseNode>   ParseRelational();
    std::unique_ptr<AST_BaseNode>   ParseAdd();
    std::unique_ptr<AST_BaseNode>   ParseMul();
    std::unique_ptr<AST_BaseNode>   ParseUnary();
    std::unique_ptr<AST_BaseNode>   ParsePrimary();

    void AddError(){
        llvm::errs() << "Unexpected: " << token.GetRefText() << "\n";
        hasError = true;
    }

    void Advance(){
        lexer.GetNext(token);
    }

    void Advance(const char* inChar){
        lexer.GetNext(token, inChar);
    }

    bool IsNextTokenOfType(Token::TokenType tokenType){
        if (!token.Is(tokenType)){
            AddError();
            return true;
        }
        return false;
    }

    bool Consume(Token::TokenType tokenType){
        if (IsNextTokenOfType(tokenType)){
            return true;
        }
        Advance();
        return false;
    }

private:
    Lexer& lexer;
    Token token;
    bool hasError;

    std::queue<AST_BaseNode*>       AST_tree;//AST队列
    std::unordered_set<std::string> LocalVar;//本地局部变量
};