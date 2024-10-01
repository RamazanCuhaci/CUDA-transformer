#ifndef CUDA_TRANSFORM_VISITOR_H
#define CUDA_AST_VISITOR_H

#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "optimizationInfo.h"

class CUDA_TransformVisitor : public clang::RecursiveASTVisitor<CUDA_TransformVisitor>
{

    clang::Rewriter &writer;
    OptimizationInfo &optimizations;

public:
    
        explicit CUDA_TransformVisitor(
            clang::Rewriter writer,
            OptimizationInfo &optimizations);
        bool VisitCUDAKernelCallExpr(clang::CUDAKernelCallExpr *kernelCall);   
        bool VisitCallExpr(clang::CallExpr *call);   
        


};


#endif