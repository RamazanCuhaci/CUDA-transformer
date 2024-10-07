#ifndef CUDA_AST_VISITOR_H
#define CUDA_AST_VISITOR_H

#include "Expressions.h"
#include "clang/AST/ASTTypeTraits.h"
#include "clang/AST/ParentMap.h"
#include "clang/AST/ParentMapContext.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include <unordered_set>


class CUDA_ASTVisitor : public clang::RecursiveASTVisitor<CUDA_ASTVisitor>
{

    clang::ASTContext *context;
    clang::Rewriter &writer;
    clang::ParentMapContext parentMapContext;

    // We aim that changed the inside the kernel function(mostly)
    // So we need the ensure the expressions is inside the kernel function not the host functions
    bool isVisitorInsideKernel;
    bool isNextIfNested; // Flag to indicate whether the next if statement is nested


    bool checkNestedIf(clang::Stmt *stmt);
    std::vector<clang::Stmt*> bodies;// Store the bodies of if-else statements

  public:
    explicit CUDA_ASTVisitor(clang::ASTContext *context, clang::Rewriter &writer, Expressions &targetExpressions);

    bool VisitCUDAKernelCallExpr(clang::CUDAKernelCallExpr *kernelCallEexpr);
    bool VisitCallExpr(clang::CallExpr *callExpr);
    bool VisitFunctionDecl(clang::FunctionDecl *funcDecl);
    bool VisitIfStmt(clang::IfStmt *ifStmt);
    Expressions &targetExpressions;
};

#endif