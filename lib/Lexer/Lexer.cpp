#include "chilang/Lexer/Lexer.h"
#include <cctype>
#include <string.h>

//添加TokenList的原因是为了将来在token中添加更多的修饰而对token提前进行生成和管理
void Lexer::TokenListGenerater(){
    Token token_buf;
    do{
        GetNextToken(token_buf);
        this->token_list.push_back(token_buf);
    }while((token_buf.GetType()!=Token::TK_EOF)&&(token_buf.GetType()!=Token::TK_UKNOWN));
}

void Lexer::GetNextToken(Token& token){
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
            llvm_unreachable("Lex:未知的符号");
        }
        return;
    }
}

void Lexer::GetNext(Token& token){
    token = *token_itor;
    token_itor++;
    return;
}

void Lexer::GetNext(Token& token, const char* inChar){
    if(token.GetRefText().compare(inChar)){
        llvm_unreachable("Lex:此符号不应出现在此位置");
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