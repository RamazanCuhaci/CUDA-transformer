#ifndef CUDA_AST_CONSUMER_H
#define CUDA_AST_CONSUMER_H

#include "CUDA_TransformVisitor.h"
#include "CUDA_AnalysisVisitor.h"
#include "clang/AST/ASTConsumer.h"



class CUDA_ASTConsumer : public clang::ASTConsumer
{   
    clang::Rewriter &writer;
    OptimizationInfo &optimizations;
    CUDA_AnalysisVisitor analysisPass; 
    CUDA_TransformVisitor transformPass;

    public:
        
        explicit CUDA_ASTConsumer(clang::ASTContext *context, clang::Rewriter &writer, OptimizationInfo &optimizations);
        virtual void HandleTranslationUnit(clang::ASTContext &Context) override;

};



#endif