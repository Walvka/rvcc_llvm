#pragma once

#include "chilang/Parser/AST.h"
#include "chilang/Parser/Parser.h"

//生成ir指令
class IRGenerator{
public:
    void Generate(Parser& parser);
};