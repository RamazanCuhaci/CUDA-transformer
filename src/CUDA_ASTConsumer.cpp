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
        transformer.analyzeSyncthread(syncCall, writer, context); // Ask for __syncthreads optimizations
    }

    for (clang::CallExpr *atomicCall : targetExpressions.atomicCalls)
    {
        transformer.analyzeAtomicCalls(atomicCall, writer, context); // Ask for atomic optimizations
    }

    // Add loops for other types of expressions as needed
    for (clang::CUDAKernelCallExpr *kernelCall : targetExpressions.kernelCalls)
    {
        transformer.analyzeKernelCall(kernelCall, writer, context); // Ask for kernel call optimizations
    }

    for (std::vector<clang::Stmt *> ifElseBody : targetExpressions.ifElseBodies)
    {
    
        transformer.analyzeIfElse(ifElseBody, writer, context); // Ask for if-else optimizations
    }

    transformer.executeCommands();
}