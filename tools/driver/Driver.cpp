#include "llvm/Support/CommandLine.h"
#include "llvm/Support/InitLLVM.h"
#include "llvm/Support/raw_ostream.h"
#include "chilang/Basic/Version.h"
#include "chilang/Lexer/Lexer.h"
#include "chilang/Parser/Parser.h"

namespace{
    class ASTPrinter : public ASTVisitor{
    public:
        virtual void Visit(AST_newNumNode& node) override{
            llvm::outs() <<  "+++++++++++++" << "\n";
            llvm::outs() <<  "NodeType:  " << node.GetNodeType()  << "\n";
            llvm::outs() <<  "NodeName:  " << node.GetNodeName()  << "\n";
            llvm::outs() << "NodeValue:  " << node.GetNodeValue() << "\n";
            llvm::outs() <<  "+++++++++++++" << "\n";
        };

        virtual void Visit(AST_newBinaryNode& node) override
        {
            llvm::outs() <<  "+++++++++++++" << "\n";
            llvm::outs() <<  "NodeType:  " << node.GetNodeType()  << "\n";
            llvm::outs() <<  "NodeName:  " << node.GetNodeName()  << "\n";
            llvm::outs() << "Symble:  " << node.GetSymble() << "\n";
            llvm::outs() << "BinaryOp: " << "\n";

            if (node.GetLeft())
            {
                node.GetLeft()->Accept(*this);
            }

            if (node.GetRight())
            {
                node.GetRight()->Accept(*this);
            }
            llvm::outs() <<  "+++++++++++++" << "\n";
        };
    };
}

static llvm::cl::opt<std::string> input(llvm::cl::Positional, llvm::cl::desc("<input expression>"), llvm::cl::init(""));

int main(int argc, const char** argv)
{
    llvm::InitLLVM llvmInitializer(argc, argv);
    llvm::cl::ParseCommandLineOptions(argc, argv, "SimpleParser - a simple code parser\n");
    llvm::outs() << "Input: \"" << input << "\"\n";

    Lexer lexer(input);
    Parser parser(lexer);
    AST_BaseNode* tree = parser.Parse();
    if (!tree || parser.HasError())
    {
        llvm::errs() << "Syntax errors occured\n";
        return 1;
    }

    ASTPrinter printer;
    tree->Accept(printer);

    return 0;
}