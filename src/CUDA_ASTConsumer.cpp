#include "CUDA_ASTConsumer.h"

CUDA_ASTConsumer::CUDA_ASTConsumer(clang::ASTContext *context, clang::Rewriter &writer, Expressions &targetExpressions,
                                   Transformer &transformer)
    : analysisVisitor(context, writer, targetExpressions), writer(writer), transformer(transformer),
      targetExpressions(targetExpressions)
{
}

void CUDA_ASTConsumer::HandleTranslationUnit(clang::ASTContext &context)
{

    analysisVisitor.TraverseDecl(context.getTranslationUnitDecl());

    // Ask user for decisions after traversal
    for (clang::CallExpr *syncCall : targetExpressions.syncthreadCalls)
    {
        transformer.analyzeSyncthread(syncCall, writer); // Ask for __syncthreads optimizations
    }

    for (clang::CallExpr *atomicCall : targetExpressions.atomicCalls)
    {
        transformer.analyzeAtomicCalls(atomicCall, writer, context); // Ask for atomic optimizations
    }

    // // Add loops for other types of expressions as needed
    // for (clang::CUDAKernelCallExpr *kernelCall : targetExpressions.kernelCalls)
    // {
    //     transformer.askUserForOptimization(kernelCall, writer, "kernel"); // Ask for kernel call optimizations
    // }

    transformer.executeCommands();
}