#ifndef CUDA_AST_CONSUMER_H
#define CUDA_AST_CONSUMER_H

#include "CUDA_ASTVisitor.h"
#include "clang/AST/ASTConsumer.h"


class CUDA_ASTConsumer : public clang::ASTConsumer
{

    CUDA_ASTVisitor Visitor;

    public:
        
        explicit CUDA_ASTConsumer(clang::ASTContext *Context);
        virtual void HandleTranslationUnit(clang::ASTContext &Context) override;

};



#endif