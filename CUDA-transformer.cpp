#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendAction.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/Support/CommandLine.h"


using namespace llvm;

static llvm::cl::OptionCategory MyToolCategory("my-tool options");

llvm::cl::list<int> DoubleIndexes("double-indexes", llvm::cl::desc("Specify the indices of the double variable occurrences to modify"), llvm::cl::value_desc("index"));




class CUDA_ASTVisitor : public clang::RecursiveASTVisitor<CUDA_ASTVisitor>
{
    // The visitor to traverse the AST
    public:
        
        explicit CUDA_ASTVisitor(clang::ASTContext *Context) : Context(Context) {}

        bool VisitCUDAKernelCallExpr(clang::CUDAKernelCallExpr *kernelCall)
        {
            llvm::outs() << "CUDA Kernel Call:\n";   
            
            for (unsigned i = 0; i < kernelCall->getNumArgs(); i++)
            {
                const clang::Expr *arg = kernelCall->getArg(i);
                arg->printPretty(llvm::outs(), nullptr, clang::PrintingPolicy(Context->getLangOpts()));
                llvm::outs() << "\n";
            }

            const clang::CallExpr *config = kernelCall->getConfig();

            if (config)
            {                
                llvm::outs() << "Execution configuration:\n";

                for (unsigned i = 0; i < config->getNumArgs(); i++)
                {
                    const clang::Expr *arg = config->getArg(i);
                    arg->printPretty(llvm::outs(), nullptr, clang::PrintingPolicy(Context->getLangOpts()));
                    llvm::outs() << "\n";
                }
            }
            return true;
        }

    private:

        clang::ASTContext *Context;

};

class CUDA_ASTConsumer : public clang::ASTConsumer
{
    public:
        
        // The generic action to be performed on the AST
        explicit CUDA_ASTConsumer(clang::ASTContext *Context) : Visitor(Context) {}

        virtual void HandleTranslationUnit(clang::ASTContext &Context) override
        {

            // Traverse the AST and perform the action
            Visitor.TraverseDecl(Context.getTranslationUnitDecl());
        }

    private:

        CUDA_ASTVisitor Visitor;    

};

class CUDA_FrontEndAction : public clang::ASTFrontendAction 
{

    // The common entry point for the tool
    public:

        virtual std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(
                                                    clang::CompilerInstance &Compiler, 
                                                    llvm::StringRef InFile) override
        {
            return std::make_unique<CUDA_ASTConsumer>(&Compiler.getASTContext());
        }                                            

}; 


int main(int argc, const char **argv)
{

    auto ExpectedParser = clang::tooling::CommonOptionsParser::create(argc, argv, MyToolCategory); // Use the fully qualified name
    if (!ExpectedParser) {
        llvm::errs() << ExpectedParser.takeError();
        return 1;
    }

    clang::tooling::CommonOptionsParser &op = ExpectedParser.get(); // Use the fully qualified name
    clang::tooling::ClangTool Tool(op.getCompilations(), op.getSourcePathList());

    auto Factory = clang::tooling::newFrontendActionFactory<CUDA_FrontEndAction>();
    int Result = Tool.run(Factory.get());
    
    if (Result != 0) {
        llvm::errs() << "Error occurred while running the tool.\n";
        return Result;
    }
    return 0;

};




