#include "chilang/Parser/Parser.h"

void Parser::Parse(){
    std::unique_ptr<AST_BaseNode> tree = nullptr;
    while(!token.Is(Token::TK_EOF)){
        tree = std::move(ParseStmt());
        this->AST_tree.push(tree.get());
        tree.release();
    }
    return;
}

bool Parser::ParseTreeIsEnd(){
    return this->AST_tree.empty();
}

std::unique_ptr<AST_BaseNode> Parser::GetParseTree(){
    std::unique_ptr<AST_BaseNode> buf = std::unique_ptr<AST_BaseNode>(this->AST_tree.front());
    this->AST_tree.pop();
    return buf;
}

std::unique_ptr<AST_BaseNode> Parser::ParseStmt(){
    return ParseExprStmt();
}

// 解析语句
// stmt = exprStmt
std::unique_ptr<AST_BaseNode> Parser::ParseExprStmt(){
    std::unique_ptr<AST_BaseNode> node = std::make_unique<AST_newUnaryNode>(AST_BaseNode::ND_EXPR_STMT, std::move(ParseExpr()));
    Advance(";");
    return node;
}

// 解析表达式
// expr = assign ";"
std::unique_ptr<AST_BaseNode> Parser::ParseExpr(){
    return ParseAssign();
}

// 解析赋值
// assign = equality ("=" assign)?
std::unique_ptr<AST_BaseNode> Parser::ParseAssign(){
    // equality
    std::unique_ptr<AST_BaseNode> left = std::move(ParseEquality());
    // 可能存在递归赋值，如a=b=1
    // ("=" assign)?
    if (token.Is(Token::TK_ASSIGN)){
        Advance();
        left = std::move(std::make_unique<AST_newBinaryNode>(AST_BaseNode::ND_ASSIGN, std::move(left), std::move(ParseAssign())));
    }
    return left;
}

// 解析相等性
// equality = relational ("==" relational | "!=" relational)*
std::unique_ptr<AST_BaseNode>  Parser::ParseEquality(){
    // relational
    std::unique_ptr<AST_BaseNode> left = std::move(ParseRelational());
    // ("==" relational | "!=" relational)*
    while(token.IsPUNCT()){
        if(token.Is(Token::TK_EQ)){
            Advance();
            std::unique_ptr<AST_BaseNode> right = std::move(ParseRelational());
            left = std::move(std::make_unique<AST_newBinaryNode>(AST_BaseNode::ND_EQ, std::move(left), std::move(right)));
        }
        else if(token.Is(Token::TK_NE)){
            Advance();
            std::unique_ptr<AST_BaseNode> right = std::move(ParseRelational());
            left = std::move(std::make_unique<AST_newBinaryNode>(AST_BaseNode::ND_NE, std::move(left), std::move(right)));
        }
        else{
            break;
        }
    }
    return left;
}

// 解析比较关系
// relational = add ("<" add | "<=" add | ">" add | ">=" add)*
std::unique_ptr<AST_BaseNode> Parser::ParseRelational(){
    // add
    std::unique_ptr<AST_BaseNode> left = std::move(ParseAdd());
    // ("<" add | "<=" add | ">" add | ">=" add)*
    while(token.IsPUNCT()){
        if(token.Is(Token::TK_LT)){
            Advance();
            std::unique_ptr<AST_BaseNode> right = std::move(ParseAdd());
            left = std::move(std::make_unique<AST_newBinaryNode>(AST_BaseNode::ND_LT, std::move(left), std::move(right)));
        }
        else if(token.Is(Token::TK_GT)){
            Advance();
            std::unique_ptr<AST_BaseNode> right = ParseAdd();
            left = std::move(std::make_unique<AST_newBinaryNode>(AST_BaseNode::ND_GT, std::move(left), std::move(right)));
        }
        else if(token.Is(Token::TK_LE)){
            Advance();
            std::unique_ptr<AST_BaseNode> right = ParseAdd();
            left = std::move(std::make_unique<AST_newBinaryNode>(AST_BaseNode::ND_LE, std::move(left), std::move(right)));
        }
        else if(token.Is(Token::TK_GE)){
            Advance();
            std::unique_ptr<AST_BaseNode> right = ParseAdd();
            left = std::move(std::make_unique<AST_newBinaryNode>(AST_BaseNode::ND_GE, std::move(left), std::move(right)));
        }
        else{
            break;
        }
    }
    return left;
}

// 解析加减
// add = mul ("+" mul | "-" mul)*
std::unique_ptr<AST_BaseNode> Parser::ParseAdd(){
    std::unique_ptr<AST_BaseNode> left = std::move(ParseMul());
    while(token.IsPUNCT()){
        if(token.Is(Token::TK_PLUS)){
            Advance();
            std::unique_ptr<AST_BaseNode> right = std::move(ParseMul());
            left = std::move(std::make_unique<AST_newBinaryNode>(AST_BaseNode::ND_ADD, std::move(left), std::move(right)));
        }
        else if(token.Is(Token::TK_MINUS)){
            Advance();
            std::unique_ptr<AST_BaseNode> right = std::move(ParseMul());
            left = std::move(std::make_unique<AST_newBinaryNode>(AST_BaseNode::ND_SUB, std::move(left), std::move(right)));
        }
        else{
            break;
        }
    }
    return left;
}

// 解析乘除
// mul = unary ("*" unary | "/" unary)*
std::unique_ptr<AST_BaseNode> Parser::ParseMul(){
    // 解析一元运算
    std::unique_ptr<AST_BaseNode> left = std::move(ParseUnary());
    while(token.IsPUNCT()){
        if(token.Is(Token::TK_MUL)){
            Advance();
            std::unique_ptr<AST_BaseNode> right = std::move(ParseUnary());
            left = std::move(std::make_unique<AST_newBinaryNode>(AST_BaseNode::ND_MUL, std::move(left), std::move(right)));
        }
        else if(token.Is(Token::TK_DIV)){
            Advance();
            std::unique_ptr<AST_BaseNode> right = std::move(ParseUnary());
            left = std::move(std::make_unique<AST_newBinaryNode>(AST_BaseNode::ND_DIV, std::move(left), std::move(right)));
        }
        else{
            break;
        }
    }
    return left;
}

//解析一元运算符
std::unique_ptr<AST_BaseNode> Parser::ParseUnary(){
    if(token.Is(Token::TK_PLUS)){
        Advance();
        std::unique_ptr<AST_BaseNode> left = std::move(ParseUnary());
        return std::make_unique<AST_newUnaryNode>(AST_BaseNode::ND_POS, std::move(left));
    }
    if(token.Is(Token::TK_MINUS)){
        Advance();
        std::unique_ptr<AST_BaseNode> left = std::move(ParseUnary());
        return std::make_unique<AST_newUnaryNode>(AST_BaseNode::ND_NEG, std::move(left));
    }
    std::unique_ptr<AST_BaseNode> left = std::move(ParsePrimary());
    return left;
}

// 解析括号、数字、变量
// primary = "(" expr ")" | ident｜ num
std::unique_ptr<AST_BaseNode> Parser::ParsePrimary(){
    std::unique_ptr<AST_BaseNode> left = nullptr;
    if(token.Is(Token::TK_LEFTPAREN)){
        Advance("(");
        left = std::move(ParseExpr());
        Advance(")");
    }
    if(token.Is(Token::TK_NUM)){
        left = std::move(std::make_unique<AST_newNumNode>(AST_BaseNode::ND_NUM, token.GetValue()));
        Advance();
    }
    if(token.Is(Token::TK_IDENT)){
        std::string var_string = token.GetRefText().str();
        std::unique_ptr<AST_newVarNode> leftbuf = std::move(std::make_unique<AST_newVarNode>(AST_BaseNode::ND_VAR, token.GetRefText()));
        if(this->LocalVar.find(var_string)==this->LocalVar.end()){
            this->LocalVar.emplace(var_string);
            leftbuf->SetIsFirstDef();
        }
        left = std::move(leftbuf);
        Advance();
    }
    return left;
}