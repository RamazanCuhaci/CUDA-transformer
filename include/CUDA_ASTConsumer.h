#ifndef CUDA_AST_CONSUMER_H
#define CUDA_AST_CONSUMER_H

#include "clang/AST/ASTConsumer.h"
#include "CUDA_ASTVisitor.h"


class CUDA_ASTConsumer : public clang::ASTConsumer
{

    CUDA_ASTVisitor analysisVisitor;
    clang::Rewriter &writer;

    public:
        explicit CUDA_ASTConsumer(clang::ASTContext *context,
        clang::Rewriter &writer, 
        OptimizationInfo &info);

        virtual void HandleTranslationUnit(clang::ASTContext &Context) override;

};



#endif