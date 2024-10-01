#include "../include/CUDA_AnalysisVisitor.h"


CUDA_AnalysisVisitor::CUDA_AnalysisVisitor(
    clang::ASTContext *context, 
    OptimizationInfo &optimizations) 
    : context(context), optimizations(optimizations) 
{}


bool CUDA_AnalysisVisitor::VisitCUDAKernelCallExpr(clang::CUDAKernelCallExpr *kernelCall)
{
    const clang::CallExpr *config = kernelCall->getConfig();
    if (config) {
        llvm::outs() << "Execution configuration:\n";

    }
    return true;

}

bool CUDA_AnalysisVisitor::VisitCallExpr(clang::CallExpr *call) {
  
    if (call->getDirectCallee()->getNameInfo().getName().getAsString() == "__syncthreads")
    {
        optimizations.syncthreadCalls.push_back(call);
    }


    return true;
}
