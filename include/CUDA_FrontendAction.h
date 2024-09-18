#ifndef CUDA_FRONTEND_ACTION_H
#define CUDA_FRONTEND_ACTION_H


#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendAction.h"

#include "CUDA_ASTConsumer.h"

class CUDA_FrontendAction : public clang::ASTFrontendAction 
{

    // The common entry point for the tool
    public:

        std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(
                                                    clang::CompilerInstance &Compiler, 
                                                    llvm::StringRef InFile) override{
                                                                    return std::make_unique<CUDA_ASTConsumer>(&Compiler.getASTContext());

                                                    }
                                                   
                                                   

}; 



#endif