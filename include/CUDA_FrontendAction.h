#ifndef CUDA_FRONTEND_ACTION_H
#define CUDA_FRONTEND_ACTION_H

#include "CUDA_ASTConsumer.h"

#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendAction.h"


class CUDA_FrontendAction : public clang::ASTFrontendAction 
{

    // The common entry point for the tool
    public:

        std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(
                                                    clang::CompilerInstance &Compiler, 
                                                    llvm::StringRef InFile) override
        {
            return std::make_unique<CUDA_ASTConsumer>(&Compiler.getASTContext());

        }
                                                   
                                                   

}; 



#endif