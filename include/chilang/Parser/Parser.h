#pragma once

#include "chilang/Parser/AST.h"
#include "chilang/Lexer/Lexer.h"

#include "llvm/Support/raw_ostream.h"

// program = stmt*
// stmt = exprStmt
// exprStmt = expr ";"
// expr = equality
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

    AST_BaseNode* Parse();

    bool HasError(){
        return hasError;
    }

private:
    AST_BaseNode*   ParseStmt();
    AST_BaseNode*   ParseExprStmt();
    AST_BaseNode*   ParseExpr();
    AST_BaseNode*   ParseEquality();
    AST_BaseNode*   ParseRelational();
    AST_BaseNode*   ParseAdd();
    AST_BaseNode*   ParseMul();
    AST_BaseNode*   ParseUnary();
    AST_BaseNode*   ParsePrimary();

    void AddError(){
        llvm::errs() << "Unexpected: " << token.GetText() << "\n";
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
};