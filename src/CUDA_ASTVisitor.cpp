#include "../include/CUDA_ASTVisitor.h"


CUDA_ASTVisitor::CUDA_ASTVisitor(clang::ASTContext *Context) : Context(Context) {}


bool CUDA_ASTVisitor::VisitCUDAKernelCallExpr(clang::CUDAKernelCallExpr *kernelCall)
{
    const clang::CallExpr *config = kernelCall->getConfig();
            if (config)
            {                
                llvm::outs() << "Execution configuration:\n";

                for (unsigned i = 0; i < config->getNumArgs(); i++)
                {
                    const clang::Expr *arg = config->getArg(i);
            
                    arg->printPretty(llvm::outs(), nullptr, clang::PrintingPolicy(Context->getLangOpts()));
                    llvm::outs() << "\n";
                }
            }
            return true;

}



        
