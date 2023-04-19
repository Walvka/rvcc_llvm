#include "chilang/Parser/Parser.h"

AST_BaseNode* Parser::Parse(){
    AST_BaseNode* tree = ParseCalc();
    // IsNextTokenOfType(Token::kEOI);
    return tree;
}

AST_BaseNode* Parser::ParseCalc()
{
    AST_BaseNode* expr;

    expr = ParseExpr();

    return expr;
}

AST_BaseNode* Parser::ParseExpr(){
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

AST_BaseNode* Parser::ParseMul(){
    AST_BaseNode* left = ParsePrimary();
    while(token.IsPUNCT()){
        if(token.Is(Token::TK_MUL)){
            Advance();
            AST_BaseNode* right = ParsePrimary();
            left = new AST_newBinaryNode(AST_BaseNode::ND_MUL, left, right);
        }
        else if(token.Is(Token::TK_DIV)){
            Advance();
            AST_BaseNode* right = ParsePrimary();
            left = new AST_newBinaryNode(AST_BaseNode::ND_DIV, left, right);
        }
        else{
            break;
        }
    }
    return left;
}

AST_BaseNode* Parser::ParsePrimary(){
    if(token.Is(Token::TK_LEFTPAREN)){
        Advance();
        return ParseExpr();
    }
    if(token.Is(Token::TK_NUM)){
        AST_BaseNode* left = new AST_newNumNode(AST_BaseNode::ND_NUM, token.GetValue());
        Advance();
        return left;
    }
}