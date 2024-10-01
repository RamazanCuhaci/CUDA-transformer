#ifndef CUDA_ANALYSIS_VISITOR_H
#define CUDA_ANALYSIS_VISITOR_H

#include "clang/AST/RecursiveASTVisitor.h"
#include "optimizationInfo.h"

class CUDA_AnalysisVisitor : public clang::RecursiveASTVisitor<CUDA_AnalysisVisitor>
{

    clang::ASTContext *context;
    OptimizationInfo &optimizations;


    public:

        explicit CUDA_AnalysisVisitor(clang::ASTContext *context, OptimizationInfo &optimizations);
        bool VisitCUDAKernelCallExpr(clang::CUDAKernelCallExpr *kernelCall);
        bool VisitCallExpr(clang::CallExpr *call);   

        const std::vector<clang::CallExpr*> & getOptimizations() const;

};


#endif