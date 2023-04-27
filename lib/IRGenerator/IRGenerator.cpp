#include <map>

#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/Support/raw_ostream.h"

#include "chilang/IRGenerator/IRGenerator.h"

using namespace llvm;

namespace
{
    class IRGeneratorImplementation : public ASTVisitor{
        Function* curfunction;
        Module* module;
        IRBuilder<> builder;
        Type* voidType;
        Type* int32Type;

        
        Constant* int32Zero;

        Value* value;

        AllocaInst *Alloca;
        std::map<StringRef, AllocaInst *> NamedValues;

    public:
        IRGeneratorImplementation(Module* inModule) :
            module(inModule),
            builder(inModule->getContext()){
            voidType = Type::getVoidTy(module->getContext());
            int32Type = Type::getInt32Ty(module->getContext());
            int32Zero = ConstantInt::get(int32Type, 0, true);
        }

        //生成一个Alloca，用来生成变量
        AllocaInst *CreateEntryBlockAlloca(Function *TheFunction, StringRef VarName){
            //IRBuilder<> TmpB(&TheFunction->getEntryBlock(), TheFunction->getEntryBlock().begin());
            return builder.CreateAlloca(Type::getInt32Ty(module->getContext()), nullptr, VarName);
        }

        void Generate(Parser& parser){
            FunctionType* mainFunctionType = FunctionType::get(int32Type, {int32Type}, false);
            Function* mainFunction = Function::Create(mainFunctionType, GlobalValue::ExternalLinkage, "main", module);
            BasicBlock* basicBlock = BasicBlock::Create(module->getContext(), "entry", mainFunction);
            curfunction = mainFunction;
            builder.SetInsertPoint(basicBlock);

            while(!parser.ParseTreeIsEnd()){
                std::unique_ptr<AST_BaseNode> tree = std::move(parser.GetParseTree());
                tree->Accept(*this);
            }

            //FunctionType* calculatorWriteFunctionType = FunctionType::get(voidType, {int32Type}, false);
            //Function* calculatorWriteFunction = Function::Create(calculatorWriteFunctionType, GlobalValue::ExternalLinkage, "CalculatorWrite", module);
            //builder.CreateCall(calculatorWriteFunctionType, calculatorWriteFunction, {value});

            builder.CreateRet(int32Zero);
        }

        void Visit(AST_newNumNode& node) override
        {
           value = ConstantInt::get(int32Type, node.GetNodeValue(), true);
           return ;
        };

        void Visit(AST_newBinaryNode& node) override
        {
            Value* left;
            Value* right;
            std::unique_ptr<AST_BaseNode> left_buf = std::move(node.GetLeft());
            if(left_buf!=nullptr){
                left_buf->Accept(*this);
                left = value;
            }
        
            std::unique_ptr<AST_BaseNode> right_buf = std::move(node.GetRight());
            if(right_buf!=nullptr){
                right_buf->Accept(*this);
                right = value;
            }
            
            switch (node.GetNodeType()){
                case AST_BaseNode::ND_ADD:
                    value = builder.CreateNSWAdd(left, right);
                    break;

                case AST_BaseNode::ND_SUB:
                    value = builder.CreateNSWSub(left, right);
                    break;

                case AST_BaseNode::ND_MUL:
                    value = builder.CreateNSWMul(left, right);
                    break;

                case AST_BaseNode::ND_DIV:
                    value = builder.CreateSDiv(left, right);
                    break;

                case AST_BaseNode::ND_ASSIGN:
                    Alloca = NamedValues[left_buf->GetNodeVar()];
                    if(right_buf!=nullptr){
                        builder.CreateStore(right, Alloca);
                    }
                    else{
                        builder.CreateStore(ConstantInt::get(int32Type, 0, true), Alloca);
                    }
                    break;

                default:
                    break;
            }
        };

        void Visit(AST_newUnaryNode& node) override{
            node.GetLeft()->Accept(*this);
            Value* left = value;

            switch (node.GetNodeType()){
                case AST_BaseNode::ND_POS:
                    value = builder.CreateNSWAdd(int32Zero, left);
                    break;

                case AST_BaseNode::ND_NEG:
                    value = builder.CreateNSWSub(int32Zero, left);
                    break;

                default:
                    break;
            }
        };

        void Visit(AST_newVarNode& node) override{
            if(NamedValues.find(node.GetNodeVar())==NamedValues.end()){
                Alloca = CreateEntryBlockAlloca(curfunction, node.GetNodeVar());
                NamedValues[node.GetNodeVar()] = Alloca;
                value = ConstantInt::get(int32Type, 0, true);
            }
            else{
                Alloca = NamedValues[node.GetNodeVar()];
                value = builder.CreateLoad(Alloca->getAllocatedType(), Alloca, node.GetNodeVar());
            }
        }

    };
} // namespace

void IRGenerator::Generate(Parser& parser){
    LLVMContext context;
    Module* module = new Module("chilang.Module", context);
    IRGeneratorImplementation generator(module);
    generator.Generate(parser);
    module->print(outs(), nullptr);
}