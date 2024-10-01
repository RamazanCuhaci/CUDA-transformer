#include "../include/CUDA_ASTConsumer.h"

CUDA_ASTConsumer::CUDA_ASTConsumer(
    clang::ASTContext *context, 
    clang::Rewriter &writer,
    OptimizationInfo &info)
    : analysisVisitor(context, writer,info), writer(writer)
    {}
        

void CUDA_ASTConsumer::HandleTranslationUnit(clang::ASTContext &Context) 
{
    
    analysisVisitor.TraverseDecl(Context.getTranslationUnitDecl());
    writer.getEditBuffer(Context.getSourceManager().getMainFileID()).write(llvm::outs());

}