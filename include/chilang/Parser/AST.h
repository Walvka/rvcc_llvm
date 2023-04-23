#pragma once

#include <queue>
#include <memory>
#include <unordered_set>
#include "llvm/ADT/SmallVector.h"
#include "llvm/ADT/StringRef.h"

class AST_BaseNode;

class AST_newBinaryNode; //二叉节点
class AST_newUnaryNode;  //单叉节点
class AST_newVarNode;    //变量节点
class AST_newNumNode;    //数字节点

class ASTVisitor
{
public:
    virtual void Visit(AST_newBinaryNode&){}
    virtual void Visit(AST_newUnaryNode&){}
    virtual void Visit(AST_newVarNode&){}
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
        ND_ASSIGN,      // 赋值
        ND_EXPR_STMT,   // 表达式语句
        ND_VAR,         // 变量
        ND_NUM,         // 整形
    };

    virtual ~AST_BaseNode(){
    }

    virtual void Accept(ASTVisitor& visitor) = 0;

    //设置节点的左枝
    void SetLeft(std::unique_ptr<AST_BaseNode> inLeftExpr){
        this->NodeLeft = std::move(inLeftExpr);
    }

    //设置节点的右枝
    void SetRight(std::unique_ptr<AST_BaseNode> inRightExpr){
        this->NodeRight = std::move(inRightExpr);
    }

    //设置节点的类型
    void SetNodeType(AST_NodeKind inType){
        this->NodeType = inType;
    }

    //设置节点的名称
    void SetNodeName(llvm::StringRef inNodename){
        this->NodeName = inNodename;
    }

    //设置节点的数值
    void SetNodeValue(int inNodevalue){
        this->NodeValue = inNodevalue;
    }

    //设置当前节点符号
    void SetSymble(llvm::StringRef inSymble){
        this->NodeSymble = inSymble;
    }

    //设置节点的变量名
    void SetNodeVar(llvm::StringRef inVar){
        this->NodeVar = inVar;
    }

    //获取节点的左枝
    std::unique_ptr<AST_BaseNode> GetLeft(){
        return std::move(this->NodeLeft);
    }

    //获取节点的右枝
    std::unique_ptr<AST_BaseNode> GetRight(){
        return std::move(this->NodeRight);
    }

    //获取节点的类型
    AST_NodeKind GetNodeType() const{
        return this->NodeType;
    }

    //获取节点的名称
    llvm::StringRef GetNodeName() const{
        return this->NodeName;
    }

    //获取节点的数值
    int GetNodeValue() const{
        return this->NodeValue;
    }

    //获取当前节点符号
    llvm::StringRef GetSymble(){
        return this->NodeSymble;
    }

    //设置节点的变量名
    llvm::StringRef GetNodeVar(){
        return this->NodeVar;
    }

private:
    std::unique_ptr<AST_BaseNode>       NodeLeft;
    std::unique_ptr<AST_BaseNode>       NodeRight;
    AST_NodeKind        NodeType;     // AST的节点类型
    llvm::StringRef     NodeSymble;   // AST的节点类型字符串

    llvm::StringRef     NodeName;     // AST的节点名称
                int     NodeValue;    // AST的节点数值

    llvm::StringRef     NodeVar;      // AST的节点变量名称
};


//二叉节点定义
class AST_newBinaryNode : public AST_BaseNode
{
public:
    AST_newBinaryNode(AST_NodeKind Nodekind, std::unique_ptr<AST_BaseNode> inLeftExpr, std::unique_ptr<AST_BaseNode> inRightExpr){
        this->SetNodeType(Nodekind);
        this->SetNodeName("Expr newBinary Node");
        this->SetLeft(std::move(inLeftExpr));
        this->SetRight(std::move(inRightExpr));
        switch (Nodekind){
            case ND_ASSIGN:
                this->SetSymble("= ASSIGN");
                break;
            case ND_ADD:
                this->SetSymble("+ ADD");
                break;
            case ND_SUB:
                this->SetSymble("- SUB");
                break;
            case ND_MUL:
                this->SetSymble("* MUL");
                break;
            case ND_DIV:
                this->SetSymble("/ DIV");
                break;
            case ND_EQ:
                this->SetSymble("== ND_EQ");
                break;
            case ND_NE:
                this->SetSymble("!= ND_NE");
                break;
            case ND_LT:
                this->SetSymble("< ND_LT");
                break;
            case ND_GT:
                this->SetSymble("> ND_GT");
                break;
            case ND_LE:
                this->SetSymble("<= ND_LE");
                break;
            case ND_GE:
                this->SetSymble(">= ND_GE");
                break;
            default:
                break;
        }
    }

    virtual void Accept(ASTVisitor& visitor) override{
        visitor.Visit(*this);
    }
};

//单叉节点定义
class AST_newUnaryNode : public AST_BaseNode
{
public:
    AST_newUnaryNode(AST_NodeKind Nodekind, std::unique_ptr<AST_BaseNode> inLeftExpr){
        this->SetNodeType(Nodekind);
        this->SetNodeName("Expr newUnary Node");
        this->SetLeft(std::move(inLeftExpr));
        this->SetRight(nullptr);
        switch (Nodekind)
        {
            case ND_EXPR_STMT:
                this->SetSymble("EXPR_STMT");
                break;
            case ND_POS:
                this->SetSymble("+_ POS");
                break;
            case ND_NEG:
                this->SetSymble("-_ NEG");
                break;
            default:
                break;
        }
    }

    virtual void Accept(ASTVisitor& visitor) override{
        visitor.Visit(*this);
    }
};

//新的变量节点定义
class AST_newVarNode : public AST_BaseNode{
public:
    AST_newVarNode(AST_NodeKind inType, llvm::StringRef inVar){
        this->SetNodeType(inType);
        this->SetNodeVar(inVar);
        this->SetNodeName("Expr newNum Node");
        first_def = 0;
    }

    virtual void Accept(ASTVisitor& visitor) override{
        visitor.Visit(*this);
    }

    void SetIsFirstDef(){first_def = 1;};
    int GetIsFirstDef(){return first_def;};

private:
    int first_def;//指明此节点是否是初次def
};

//新的数字节点定义
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