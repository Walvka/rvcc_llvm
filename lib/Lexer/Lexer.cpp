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
        case '-':
            InitializeToken(token, bufferPtr + 1, Token::TK_PUNCT);
            break;
        default:
            InitializeToken(token, bufferPtr + 1, Token::TK_UKNOWN);
        }
        return;
    }
}

void Lexer::InitializeToken(Token& token, const char* tokenEnd, Token::TokenType type){
    token.SetType(type);
    token.SetText(llvm::StringRef(bufferPtr, tokenEnd - bufferPtr));
    bufferPtr = tokenEnd;
}