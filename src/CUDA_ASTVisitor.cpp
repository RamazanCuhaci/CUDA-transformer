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

bool CUDA_ASTVisitor::VisitCallExpr(clang::CallExpr *callExpr)
{

    if (const clang::FunctionDecl *callee = callExpr->getDirectCallee())
    {
        if (callee->getNameInfo().getAsString() == "__syncthreads")
        {
            targetExpressions->syncthreadCalls.push_back(callExpr);
        }
    }

    return true;
}
