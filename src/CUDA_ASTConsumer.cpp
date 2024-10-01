#include "../include/CUDA_ASTConsumer.h"

CUDA_ASTConsumer::CUDA_ASTConsumer(clang::ASTContext *context,clang::Rewriter &writer, OptimizationInfo &optimizations) 
: writer(writer), analysisPass(context,optimizations), transformPass(writer,optimizations), optimizations(optimizations)
{}

void CUDA_ASTConsumer::HandleTranslationUnit(clang::ASTContext &Context)
{

    analysisPass.TraverseDecl(Context.getTranslationUnitDecl());

    // Interact with the user: Ask them what they want to do with the optimization possibilities
    for (auto Call : optimizations.syncthreadCalls) {
        llvm::outs() << "Found __syncthreads at: " << Call->getBeginLoc().printToString(Context.getSourceManager()) << "\n";
        llvm::outs() << "Choose an option: (1) Replace with __syncwarp, (2) Delete, (3) Do nothing\n";
        // Assume user choice for now
    }

    // Run the transformation pass next
    transformPass.TraverseDecl(Context.getTranslationUnitDecl());

}