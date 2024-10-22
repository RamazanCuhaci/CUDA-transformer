#ifndef CUDA_AST_CONSUMER_H
#define CUDA_AST_CONSUMER_H

#include "CUDA_ASTVisitor.h"
#include "Transformer.h"
#include "clang/AST/ASTConsumer.h"

class CUDA_ASTConsumer : public clang::ASTConsumer
{

    CUDA_ASTVisitor analysisVisitor;
    clang::Rewriter &writer;
    Transformer &transformer;
    clang::FileID mainFileID;
    
  public:
    explicit CUDA_ASTConsumer(clang::ASTContext *context, clang::Rewriter &writer, Expressions &targetExpressions,
                              Transformer &transformer, clang::FileID mainFileID);

    virtual void HandleTranslationUnit(clang::ASTContext &Context) override;
    void applyOptimizationsChoices(clang::ASTContext &context); 
    Expressions &targetExpressions;
    void printOptimizationPossibilities();
};

#endif