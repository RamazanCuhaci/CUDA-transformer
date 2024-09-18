#include "../include/CUDA_ASTConsumer.h"

CUDA_ASTConsumer::CUDA_ASTConsumer(clang::ASTContext *Context) : Visitor(Context) {}

void CUDA_ASTConsumer::HandleTranslationUnit(clang::ASTContext &Context) 
{

    Visitor.TraverseDecl(Context.getTranslationUnitDecl());

}