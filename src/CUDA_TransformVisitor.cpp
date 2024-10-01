#include "../include/CUDA_TransformVisitor.h"

CUDA_TransformVisitor::CUDA_TransformVisitor(
    clang::Rewriter writer,
    OptimizationInfo &optimizations) 
: writer(writer), optimizations(optimizations) 
{}


bool CUDA_TransformVisitor::VisitCUDAKernelCallExpr(clang::CUDAKernelCallExpr *kernelCall)
{
    const clang::CallExpr *config = kernelCall->getConfig();
    if (config) 
    {
        llvm::outs() << "Execution configuration:\n";

    }
    return true;

}

bool CUDA_TransformVisitor::VisitCallExpr(clang::CallExpr *call) {
  
  if (std::find(
    optimizations.syncthreadCalls.begin(), 
    optimizations.syncthreadCalls.end(), call) != optimizations.syncthreadCalls.end()) 
    {
        // Replace __syncthreads with __syncwarp or delete based on user choice
        writer.ReplaceText(call->getSourceRange(), "__syncwarp()");
    }

  return true;

}
