#include "../include/CUDA_ASTVisitor.h"

CUDA_ASTVisitor::CUDA_ASTVisitor(clang::ASTContext *context, clang::Rewriter &writer, Expressions *targetExpressions)
    : context(context), writer(writer), targetExpressions(targetExpressions)
{
}

bool CUDA_ASTVisitor::VisitCUDAKernelCallExpr(clang::CUDAKernelCallExpr *kernelCall)
{
    targetExpressions->kernelCalls.push_back(kernelCall);
    return true;
}

