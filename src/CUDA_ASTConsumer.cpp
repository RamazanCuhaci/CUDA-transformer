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

    // Print optimization possibilities
    printOptimizationPossibilities();
    transformer.getOptimizationChoices();
    applyOptimizationsChoices(context);

    transformer.executeCommands();
}

void CUDA_ASTConsumer::applyOptimizationsChoices(clang::ASTContext &context)
{
    /// IMPORTANT : ORDER OF APPLYING IS IMPORTANT  (MUST BE SAME AS PRINTING)

    /// IMPORTANT : When If Else body has other optimization possibilities
    /// TODO : Implement a better way to handle this

    for (clang::CallExpr *syncCall : targetExpressions.syncthreadCalls)
    {
        transformer.analyzeSyncthread(syncCall, writer, context); 
    }

    for (clang::CallExpr *atomicCall : targetExpressions.atomicCalls)
    {
        transformer.analyzeAtomicCalls(atomicCall, writer, context);
    }

    for (clang::TypeLoc doubleTypeLoc : targetExpressions.doubles)
    {
        transformer.analyzeDoubles(doubleTypeLoc, writer);
    }

    for (clang::CUDAKernelCallExpr *kernelCall : targetExpressions.kernelCalls)
    {

        transformer.analyzeKernelCall(kernelCall, writer, context); 
    }

    for (std::vector<clang::Stmt *> ifElseBody : targetExpressions.ifElseBodies)
    {

        transformer.analyzeIfElse(ifElseBody, writer, context,
                                  targetExpressions.ifElseSourceRange);
    }

    for (clang::ForStmt *forStmt : targetExpressions.forStmts)
    {
        transformer.analyzeForStmt(forStmt, writer, context);
    }

    for (clang::ForStmt *kernelLaunchforStmt : targetExpressions.kernelLaunchforStmts)
    {
        transformer.analyzeForStmt(kernelLaunchforStmt, writer, context);
    }
}

void CUDA_ASTConsumer::printOptimizationPossibilities()
{

    /// IMPORTANT : ORDER OF PRINTING IS IMPORTANT

    std::cout << "Optimization possibilities: \n";
    
    for (size_t i = 0; i < targetExpressions.syncthreadCalls.size(); i++)
    {
        
        std::cout << 0;
    }

    for (size_t i = 0; i < targetExpressions.atomicCalls.size(); i++)
    {
        std::cout << 1;
    }

    for (size_t i = 0; i < targetExpressions.doubles.size(); i++)
    {
        std::cout << 2;
    }

    for (size_t i = 0; i < targetExpressions.kernelCalls.size(); i++)
    {
        std::cout << 3 << 4;
    }
    
    for (std::vector<clang::Stmt *> ifElseBody : targetExpressions.ifElseBodies)
    {
        if (ifElseBody.size() == 1)
        {
            std::cout << 5;
        }
        else if (ifElseBody.size() == 2)
        {
            std::cout << 6;
        }
        else if (ifElseBody.size() == 3)
        {
            std::cout << 7;
        }
        else
        {
            llvm::errs() << "Error: If-Else body size is not supported\n";
        }
    }

    for (size_t i = 0; i < targetExpressions.kernelLaunchforStmts.size(); i++)
    {
        std::cout << 8;
    }

     for (size_t i = 0; i < targetExpressions.forStmts.size(); i++)
    {
        std::cout << 9;
    }

    std::cout << "\n";
}