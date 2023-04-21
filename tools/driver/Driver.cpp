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

            std::unique_ptr<AST_BaseNode> left_buf = std::move(node.GetLeft());
            std::unique_ptr<AST_BaseNode> right_buf = std::move(node.GetRight());
            if(left_buf!=nullptr){
                llvm::outs() << "Left" << "\n";
                left_buf->Accept(*this);
            }
            if(right_buf!=nullptr){
                llvm::outs() << "Left" << "\n";
                right_buf->Accept(*this);
            }
            llvm::outs() <<  "+++++++++++++" << "\n";
        };
    };
}

static llvm::cl::opt<std::string> input(llvm::cl::Positional, llvm::cl::desc("<input expression>"), llvm::cl::init(""));

int main(int argc, const char** argv)
{
    llvm::InitLLVM llvmInitializer(argc, argv);
    llvm::cl::ParseCommandLineOptions(argc, argv, "ChilangCompiler  - a simple code compiler\n");
    llvm::outs() << "Input: \"" << input << "\"\n";

    Lexer lexer(input);
    Parser parser(lexer);
    std::unique_ptr<AST_BaseNode> tree = std::move(parser.Parse());
    if (!tree || parser.HasError())
    {
        llvm::errs() << "Syntax errors occured\n";
        return 1;
    }

    ASTPrinter printer;
    tree->Accept(printer);

    return 0;
}