#ifndef CUDA_AST_VISITOR_H
#define CUDA_AST_VISITOR_H

#include "clang/AST/RecursiveASTVisitor.h"

class CUDA_ASTVisitor : public clang::RecursiveASTVisitor<CUDA_ASTVisitor>
{

    clang::ASTContext *Context;

    public:
    
        explicit CUDA_ASTVisitor(clang::ASTContext *Context);


        bool VisitCUDAKernelCallExpr(clang::CUDAKernelCallExpr *kernelCall);   

};


#endif