#include "CUDA_ASTVisitor.h"

CUDA_ASTVisitor::CUDA_ASTVisitor(clang::ASTContext *context, clang::Rewriter &writer, Expressions &targetExpressions)
    : context(context), writer(writer), targetExpressions(targetExpressions), isVisitorInsideKernel(false)
{
}

bool CUDA_ASTVisitor::VisitFunctionDecl(clang::FunctionDecl *funcDecl)
{
    if (funcDecl->hasAttr<clang::CUDAGlobalAttr>())
    {
        isVisitorInsideKernel = true;
    }
    else
    {
        isVisitorInsideKernel = false;
    }

    return true;
}

bool CUDA_ASTVisitor::VisitCUDAKernelCallExpr(clang::CUDAKernelCallExpr *kernelCall)
{
    targetExpressions.kernelCalls.push_back(kernelCall);

    return true;
}

bool CUDA_ASTVisitor::VisitCallExpr(clang::CallExpr *callExpr)
{
    // Change expressions that only inside the kernel function
    if (isVisitorInsideKernel)
    {
        // Check for null pointer
        if (const clang::FunctionDecl *callee = callExpr->getDirectCallee())
        {
            if (callee->getNameAsString() == "__syncthreads")
            {
                targetExpressions.syncthreadCalls.push_back(callExpr);
            }
            else if (callee->getNameAsString().substr(0, 6) == "atomic")
            {
                targetExpressions.atomicCalls.push_back(callExpr);
            }
        }
    }

    return true;
}
