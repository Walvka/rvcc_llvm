#pragma once

#include "llvm/ADT/SmallVector.h"
#include "llvm/ADT/StringRef.h"


class AST_BaseNode;

class AST_newBinaryNode;
class AST_newNumNode;

class ASTVisitor
{
public:
    virtual void Visit(AST_newBinaryNode&){}
    virtual void Visit(AST_newNumNode&){}
};

class AST_BaseNode{
public:
    enum AST_NodeKind{
        ND_EXP,         // 表达式节点
        ND_ADD,         // 加操作节点
        ND_SUB,         // 减操作节点
        ND_MUL,         // 乘操作节点
        ND_DIV,         // 除操作节点
        ND_POS,         // 正号+
        ND_NEG,         // 负号-
        ND_EQ,          // ==
        ND_NE,          // !=
        ND_LT,          // <
        ND_GT,          // >
        ND_LE,          // <=
        ND_GE,          // >=
        ND_EXPR_STMT,   // 表达式语句
        ND_NUM,         // 整形
    };

    virtual ~AST_BaseNode(){
    }

    virtual void Accept(ASTVisitor& visitor) = 0;

    //设置节点的左枝
    void SetLeft(AST_BaseNode* inLeftExpr){
        this->left = inLeftExpr;
    }

    //设置节点的右枝
    void SetRight(AST_BaseNode* inRightExpr){
        this->right = inRightExpr;
    }

    //设置节点的类型
    void SetNodeType(AST_NodeKind inType){
        this->nodetype = inType;
    }

    //设置节点的名称
    void SetNodeName(llvm::StringRef inNodename){
        this->nodename = inNodename;
    }

    //设置节点的数值
    void SetNodeValue(int inNodevalue){
        this->nodevalue = inNodevalue;
    }

    //设置当前节点符号
    void SetSymble(llvm::StringRef inSymble){
        this->symble = inSymble;
    }

    //获取节点的左枝
    AST_BaseNode* GetLeft(){
        return this->left;
    }

    //获取节点的右枝
    AST_BaseNode* GetRight(){
        return this->right;
    }

    //获取节点的类型
    AST_NodeKind GetNodeType() const{
        return this->nodetype;
    }

    //获取节点的名称
    llvm::StringRef GetNodeName() const{
        return this->nodename;
    }

    //获取节点的数值
    int GetNodeValue() const{
        return this->nodevalue;
    }

    //获取当前节点符号
    llvm::StringRef GetSymble(){
        return this->symble;
    }

private:
    AST_BaseNode*       left;
    AST_BaseNode*       right;
    AST_NodeKind        nodetype;     // AST的节点类型
    llvm::StringRef     symble;       // AST的节点类型字符串

    llvm::StringRef     nodename;     // AST的节点名称
                int     nodevalue;    // AST的节点数值
    
};


class AST_newBinaryNode : public AST_BaseNode
{
public:
    AST_newBinaryNode(AST_NodeKind Nodekind, AST_BaseNode* inLeftExpr, AST_BaseNode* inRightExpr){
        SetNodeType(Nodekind);
        SetNodeName("Expr newBinary Node");
        SetLeft(inLeftExpr);
        SetRight(inRightExpr);
        switch (Nodekind)
        {
            case ND_ADD:
                SetSymble("+ ADD");
                break;
            case ND_SUB:
                SetSymble("- SUB");
                break;
            case ND_POS:
                SetSymble("+_ POS");
                break;
            case ND_NEG:
                SetSymble("-_ NEG");
                break;
            case ND_MUL:
                SetSymble("* MUL");
                break;
            case ND_DIV:
                SetSymble("/ DIV");
                break;
            case ND_EQ:
                SetSymble("== ND_EQ");
                break;
            case ND_NE:
                SetSymble("!= ND_NE");
                break;
            case ND_LT:
                SetSymble("< ND_LT");
                break;
            case ND_GT:
                SetSymble("> ND_GT");
                break;
            case ND_LE:
                SetSymble("<= ND_LE");
                break;
            case ND_GE:
                SetSymble(">= ND_GE");
                break;
            case ND_EXPR_STMT:
                SetSymble("EXPR_STMT");
                break;
            default:
                break;
        }
    }

    virtual void Accept(ASTVisitor& visitor) override{
        visitor.Visit(*this);
    }

private:
    llvm::StringRef     symble;
};

class AST_newNumNode : public AST_BaseNode{
public:
    AST_newNumNode(AST_NodeKind inType, int inVal){
        this->SetNodeType(inType);
        this->SetNodeValue(inVal);
        this->SetNodeName("Expr newNum Node");
    }

    virtual void Accept(ASTVisitor& visitor) override{
        visitor.Visit(*this);
    }
};