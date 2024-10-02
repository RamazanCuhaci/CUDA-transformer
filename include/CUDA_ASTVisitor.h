#ifndef CUDA_AST_VISITOR_H
#define CUDA_AST_VISITOR_H

#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Basic/SourceManager.h"
#include "Expressions.h"

class CUDA_ASTVisitor : public clang::RecursiveASTVisitor<CUDA_ASTVisitor>
{

    clang::ASTContext *context;
    clang::Rewriter &writer;
    const Expressions *targetExpressions;

public:

    explicit CUDA_ASTVisitor(
        clang::ASTContext *context, 
        clang::Rewriter &writer,
        const Expressions *targetExpressions);

    bool VisitCUDAKernelCallExpr(clang::CUDAKernelCallExpr *kernelCall);   
    const Expressions getExpressions();

        

};


#endif