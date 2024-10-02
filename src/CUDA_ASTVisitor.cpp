#include "../include/CUDA_ASTVisitor.h"


CUDA_ASTVisitor::CUDA_ASTVisitor(
    clang::ASTContext *context, 
    clang::Rewriter &writer,
    const Expressions *targetExpressions) 
    : context(context), writer(writer), targetExpressions(targetExpressions) 
    {}


bool CUDA_ASTVisitor::VisitCUDAKernelCallExpr(clang::CUDAKernelCallExpr *kernelCall)
{
    
    const clang::CallExpr *config = kernelCall->getConfig();
    if (config)
    {                
        llvm::outs() << "Execution configuration:\n";

        for (unsigned i = 0; i < config->getNumArgs(); i++)
        {
            const clang::Expr *arg = config->getArg(i);
    
            arg->printPretty(
                llvm::outs(), 
                nullptr, 
                clang::PrintingPolicy(context->getLangOpts()));
            llvm::outs() << "\n";
        }
    }

    // targetExpressions->kernelCalls.push_back(kernelCall);

    return true;

}



        