#include "chilang/Lexer/Lexer.h"
#include <cctype>

void Lexer::GetNext(Token& token){
    while (*bufferPtr && std::isspace(*bufferPtr)){
        ++bufferPtr;
    }

    if (!*bufferPtr){
        token.SetType(Token::TK_EOF);
        return;
    }

    if (std::isdigit(*bufferPtr)){
        const char* end = bufferPtr + 1;
        while (std::isdigit(*end)){
            ++end;
        }
        InitializeToken(token, end, Token::TK_NUM);
        return;
    }
    else{
        switch (*bufferPtr){
        case '+':
            InitializeToken(token, bufferPtr + 1, Token::TK_PLUS);
            break;
        case '-':
            InitializeToken(token, bufferPtr + 1, Token::TK_MINUS);
            break;
        case '*':
            InitializeToken(token, bufferPtr + 1, Token::TK_MUL);
            break;
        case '/':
            InitializeToken(token, bufferPtr + 1, Token::TK_DIV);
            break;
        case '(':
            InitializeToken(token, bufferPtr + 1, Token::TK_LEFTPAREN);
            break;
        case ')':
            InitializeToken(token, bufferPtr + 1, Token::TK_RIGHTPAREN);
            break;
        default:
            InitializeToken(token, bufferPtr + 1, Token::TK_UKNOWN);
        }
        return;
    }
}

void Lexer::InitializeToken(Token& token, const char* tokenEnd, Token::TokenType type){
    char* buf;
    token.SetType(type);
    token.SetText(llvm::StringRef(bufferPtr, tokenEnd - bufferPtr));
    if(type==Token::TK_NUM){
        token.SetValue(strtoul(bufferPtr, &buf, 10));
    }
    bufferPtr = tokenEnd;
}