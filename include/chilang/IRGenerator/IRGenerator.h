#pragma once

#include "chilang/Parser/AST.h"

class IRGenerator{
public:
    void Generate(std::unique_ptr<AST_BaseNode> tree);
};