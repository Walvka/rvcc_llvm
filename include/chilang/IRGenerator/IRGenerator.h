#pragma once

#include "chilang/Parser/AST.h"

class IRGenerator{
public:
    void Generate(AST_BaseNode* Tree);
};