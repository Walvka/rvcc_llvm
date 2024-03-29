#pragma once

#include <list>
#include "llvm/Support/ErrorHandling.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/Support/MemoryBuffer.h"

class Token{
public:
    //
    // 终结符分析，词法分析
    //

    // 为每个终结符都设置种类来表示
    enum TokenType{
        TK_IDENT,       // 标记符，可以为变量名、函数名等
        TK_PLUS,        // 运算符 +
        TK_MINUS,       // 运算符 -
        TK_MUL,         // 运算符 *
        TK_DIV,         // 运算符 /
        TK_EQ,          // ==
        TK_NE,          // !=
        TK_LT,          // <
        TK_GT,          // >
        TK_LE,          // <=
        TK_GE,          // >=
        TK_ASSIGN,      // 赋值
        TK_COMMA,       // 逗号 ,
        TK_COLON,       // 冒号 :
        TK_SEMI,        // 分号 ;
        TK_LEFTPAREN,   // 左圆括号 (
        TK_RIGHTPAREN,  // 右圆括号 )
        TK_NUM,         // 数字
        TK_EOF,         // 文件终止符，即文件的最后
        TK_UKNOWN,      // 未知符号
    };

public:
    //设置Token的类型
    void SetType(TokenType inType){
        type = inType;
    }

    //设置Token的字符串
    void SetRefText(llvm::StringRef inText){
        text = inText;
    }

    //设置Token的数值
    void SetValue(int inValue){
        value = inValue;
    }

    //获取Token的类型
    TokenType GetType() const{
        return type;
    }

    //获取Token的字符串
    llvm::StringRef GetRefText() const{
        return text;
    }

    //获取Token的数值
    int GetValue() const{
        return value;
    }

    //判断当前Token类型是否是一个运算操作
    bool IsPUNCT() const{
        return ((type >= TK_PLUS) && (type < TK_COMMA));
    }

    //判断当前Token类型是否与提供的类型相同
    bool Is(TokenType inType) const{
        return type == inType;
    }

    //判断当前Token类型是否在提供的类型之中
    bool IsOneOf(TokenType type1, TokenType type2) const{
        return Is(type1) || Is(type2);
    }

    //判断当前Token类型是否在提供的类型之中，模板版本
    template<typename... Ts> bool IsOneOf(TokenType type1, TokenType type2, Ts... types) const{
        return Is(type1) || IsOneOf(type2, types...);
    }

private:
    TokenType type;         // 终结符类型
    llvm::StringRef text;   // 终结符字符串
    int       value;        // 终结符数值
};

class Lexer{
public:
    Lexer(const llvm::StringRef& Buffer){
        bufferStart = Buffer.begin();
        bufferPtr = bufferStart;
        TokenListGenerater();
        token_itor = token_list.begin();
    }

    //获取下一个Token
    void GetNext(Token& token);

    //获取下一个指定的Token
    void GetNext(Token& token, const char* inChar);

private:
    //初始化新的Token
    void InitializeToken(Token& token, const char* tokenEnd, Token::TokenType type);

private:
    void TokenListGenerater();  //tokenlist生成器
    void GetNextToken(Token& token);//生成token
    const char* bufferStart;   //输入字符串的起始指针
    const char* bufferPtr;     //输入字符串的当前位置指针
    
    std::list<Token> token_list; //token列表
    std::list<Token>::iterator token_itor;//token列表迭代器
};