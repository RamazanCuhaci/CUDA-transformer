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

    // We aim that changed the inside the kernel function(mostly)
    // So we need the ensure the expressions is inside the kernel function not the host functions
    bool isVisitorInsideKernel{};

  public:
    explicit CUDA_ASTVisitor(clang::ASTContext *context, clang::Rewriter &writer, Expressions &targetExpressions);

    bool VisitCUDAKernelCallExpr(clang::CUDAKernelCallExpr *kernelCallEexpr);
    bool VisitCallExpr(clang::CallExpr *callExpr);
    bool VisitFunctionDecl(clang::FunctionDecl *funcDecl);

    Expressions &targetExpressions;
};

#endif