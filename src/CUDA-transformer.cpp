#include "../include/CUDA_FrontendAction.h"

#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/Support/CommandLine.h"
           


static llvm::cl::OptionCategory MyToolCategory("my-tool options");

llvm::cl::list<int> DoubleIndexes("double-indexes", llvm::cl::desc("Specify the indices of the double variable occurrences to modify"), llvm::cl::value_desc("index"));


int main(int argc, const char **argv)
{

    auto ExpectedParser = clang::tooling::CommonOptionsParser::create(argc, argv, MyToolCategory); // Use the fully qualified name
    if (!ExpectedParser) 
    {
        llvm::errs() << ExpectedParser.takeError();
        return 1;
    }

    clang::tooling::CommonOptionsParser &op = ExpectedParser.get(); // Use the fully qualified name
    clang::tooling::ClangTool Tool(op.getCompilations(), op.getSourcePathList());

    auto Factory = clang::tooling::newFrontendActionFactory<CUDA_FrontendAction>();
    int Result = Tool.run(Factory.get());
    
    if (Result != 0)
    {
        llvm::errs() << "Error occurred while running the tool.\n";
        return Result;
    }
    return 0;

};




