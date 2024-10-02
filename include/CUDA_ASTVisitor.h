#ifndef CUDA_AST_VISITOR_H
#define CUDA_AST_VISITOR_H

#include "Expressions.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Rewrite/Core/Rewriter.h"

class CUDA_ASTVisitor : public clang::RecursiveASTVisitor<CUDA_ASTVisitor>
{

    clang::ASTContext *context;
    clang::Rewriter &writer;

  public:
    explicit CUDA_ASTVisitor(clang::ASTContext *context, clang::Rewriter &writer, Expressions *targetExpressions);

    bool VisitCUDAKernelCallExpr(clang::CUDAKernelCallExpr *kernelCallEexpr);
    bool VisitCallExpr(clang::CallExpr *callExpr);

    Expressions *targetExpressions;
};

#endif