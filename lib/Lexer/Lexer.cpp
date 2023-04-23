#include "chilang/Lexer/Lexer.h"
#include <cctype>
#include <string.h>

void Lexer::GetNext(Token& token){
    while (*bufferPtr && std::isspace(*bufferPtr)){
        ++bufferPtr;
    }

    if(!strncmp(bufferPtr, ";", strlen(";"))){
        InitializeToken(token, bufferPtr + 1, Token::TK_SEMI);
        return;
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
    else if(std::isalpha(*bufferPtr)||(!strncmp(bufferPtr, "_", strlen("_")))){
        const char* end = bufferPtr + 1;
            while(std::isdigit(*end)||std::isalpha(*end)||(!strncmp(end, "_", strlen("_")))){
            ++end;
        }
        InitializeToken(token, end, Token::TK_IDENT);
        return;
    }
    else{
        if(!strncmp(bufferPtr, "==", strlen("=="))){
            InitializeToken(token, bufferPtr + 2, Token::TK_EQ);
            return;
        }
        else if(!strncmp(bufferPtr, "!=", strlen("!="))){
            InitializeToken(token, bufferPtr + 2, Token::TK_NE);
            return;
        }
        else if(!strncmp(bufferPtr, "<=", strlen("<="))){
            InitializeToken(token, bufferPtr + 2, Token::TK_LE);
            return;
        }
        else if(!strncmp(bufferPtr, ">=", strlen(">="))){
            InitializeToken(token, bufferPtr + 2, Token::TK_GE);
            return;
        }
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
        case '<':
            InitializeToken(token, bufferPtr + 1, Token::TK_LT);
            break;
        case '>':
            InitializeToken(token, bufferPtr + 1, Token::TK_GT);
            break;
        case '(':
            InitializeToken(token, bufferPtr + 1, Token::TK_LEFTPAREN);
            break;
        case ')':
            InitializeToken(token, bufferPtr + 1, Token::TK_RIGHTPAREN);
            break;
        case '=':
            InitializeToken(token, bufferPtr + 1, Token::TK_ASSIGN);
            break;
        default:
            InitializeToken(token, bufferPtr + 1, Token::TK_UKNOWN);
        }
        return;
    }
}

void Lexer::GetNext(Token& token, const char* inChar){
    if(strncmp(bufferPtr, inChar, strlen(inChar))){
        //error
    }
    this->GetNext(token);
}

void Lexer::InitializeToken(Token& token, const char* tokenEnd, Token::TokenType type){
    char* buf;
    token.SetType(type);
    token.SetRefText(llvm::StringRef(bufferPtr, tokenEnd - bufferPtr));
    if(type==Token::TK_NUM){
        token.SetValue(strtoul(bufferPtr, &buf, 10));
    }
    bufferPtr = tokenEnd;
}