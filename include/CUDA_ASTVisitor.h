#ifndef CUDA_AST_VISITOR_H
#define CUDA_AST_VISITOR_H

#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Basic/SourceManager.h"
#include "OptimizationInfo.h"

class CUDA_ASTVisitor : public clang::RecursiveASTVisitor<CUDA_ASTVisitor>
{

    clang::ASTContext *context;
    clang::Rewriter &writer;
    OptimizationInfo &info;

    public:
    
        explicit CUDA_ASTVisitor(
            clang::ASTContext *context, 
            clang::Rewriter &writer,
            OptimizationInfo &info);

        bool VisitCUDAKernelCallExpr(clang::CUDAKernelCallExpr *kernelCall);   

};


#endif