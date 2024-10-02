#include "../include/CUDA_ASTConsumer.h"

CUDA_ASTConsumer::CUDA_ASTConsumer(clang::ASTContext *context, clang::Rewriter &writer, Expressions *targetExpressions)
    : analysisVisitor(context, writer, targetExpressions), writer(writer)
{
}

void CUDA_ASTConsumer::HandleTranslationUnit(clang::ASTContext &context)
{

    analysisVisitor.TraverseDecl(context.getTranslationUnitDecl());

}