#pragma once

#include "chilang/Parser/AST.h"
#include "chilang/Lexer/Lexer.h"

#include "llvm/Support/raw_ostream.h"

// expr = mul ("+" mul | "-" mul)*
// mul = primary ("*" primary | "/" primary)*
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

    AST_BaseNode*   ParseCalc();
    AST_BaseNode*   ParseExpr();
    AST_BaseNode*   ParseMul();
    AST_BaseNode*   ParsePrimary();

    void AddError(){
        llvm::errs() << "Unexpected: " << token.GetText() << "\n";
        hasError = true;
    }

    void Advance(){
        lexer.GetNext(token);
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