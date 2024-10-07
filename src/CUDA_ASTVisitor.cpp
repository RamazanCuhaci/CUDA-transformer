#include "CUDA_ASTVisitor.h"

CUDA_ASTVisitor::CUDA_ASTVisitor(clang::ASTContext *context, clang::Rewriter &writer, Expressions &targetExpressions)
    : context(context), writer(writer), parentMapContext(*context), isVisitorInsideKernel(false), isNextIfNested(false),
      targetExpressions(targetExpressions)
{
}

bool CUDA_ASTVisitor::VisitFunctionDecl(clang::FunctionDecl *funcDecl)
{
    if (!funcDecl)
    {
        return false;
    }

    if (funcDecl->hasAttr<clang::CUDAGlobalAttr>())
    {
        isVisitorInsideKernel = true;

        // Catch the body of the kernel function
        clang::CompoundStmt *body = llvm::dyn_cast<clang::CompoundStmt>(funcDecl->getBody());

        if (!body)
        {
            return false;
        }

        if (body && !body->body_empty())
        {
            std::unordered_set<clang::Stmt *> processedStatements;

            for (auto *item : body->body())
            {
                if (!item)
                {
                    llvm::errs() << "item is null" << "\n";
                    continue;
                }

                if (auto *ifStmt = llvm::dyn_cast<clang::IfStmt>(item))
                {

                    if (processedStatements.find(ifStmt) != processedStatements.end())
                    {
                        continue; // Skip already processed statements
                    }

                    std::vector<clang::Stmt *> currentBranch;
                    currentBranch.push_back(ifStmt);
                    processedStatements.insert(ifStmt);

                    clang::Stmt *elseStmt = ifStmt->getElse();

                    while (elseStmt)
                    {
                        if (auto *elseIfStmt = llvm::dyn_cast<clang::IfStmt>(elseStmt))
                        {
                            currentBranch.push_back(elseIfStmt);
                            processedStatements.insert(elseIfStmt);
                            elseStmt = elseIfStmt->getElse();
                        }
                        else
                        {

                            currentBranch.push_back(elseStmt);
                            processedStatements.insert(elseStmt);

                            // llvm::errs() << "Last Else Statement Found:\n";
                            // elseStmt->dump();

                            break;
                        }
                    }

                    targetExpressions.ifElseBodies.push_back(currentBranch);
                }
            }
        }
    }
    else
    {
        isVisitorInsideKernel = false;
    }
    return true;
}

bool CUDA_ASTVisitor::checkNestedIf(clang::Stmt *stmt)
{
    if (!stmt)
    {
        return false;
    }

    for (auto child : stmt->children())
    {
        if (auto nestedIf = llvm::dyn_cast<clang::IfStmt>(child))
        {
            llvm::errs() << "Nested If Statement Found:\n";
            isNextIfNested = true;
            return true;
        }
    }
    return false;
}

bool CUDA_ASTVisitor::VisitIfStmt(clang::IfStmt *ifStmt)
{

    // // Check if the visitor is currently inside a kernel
    // if (isVisitorInsideKernel)
    // {
    //     // If the current if statement is nested, do not add its body to the vector
    //     if (isNextIfNested) {
    //         isNextIfNested = false; // Reset the flag
    //         llvm::errs() << "Nested If Statement Detected, Skipping\n";
    //         return true; // Continue visiting other nodes
    //     }

    //     // Add the then body to the vector
    //     if (ifStmt->getThen()) {
    //         bodies.push_back(ifStmt->getThen());
    //         llvm::errs() << "Top-level If Statement Body Added\n";
    //     }

    //     // Add the else body to the vector
    //     if (ifStmt->getElse()) {
    //         bodies.push_back(ifStmt->getElse());
    //         llvm::errs() << "Top-level Else Statement Body Added\n";
    //     }

    //     // Check for nested if statements
    //     if (ifStmt->getThen()) {
    //         checkNestedIf(ifStmt->getThen());
    //     }
    //     if (ifStmt->getElse()) {
    //         checkNestedIf(ifStmt->getElse());
    //     }

    //     targetExpressions.ifElseBodies.push_back(bodies);

    //     // static int visitCount = 0;
    //     // llvm::errs() << "Visit Count: " << ++visitCount << "\n";

    //     const auto &parents = context->getParents(*ifStmt);

    //     auto st = parents[0].get<clang::Stmt>();
    //     clang::PrintingPolicy policy(context->getLangOpts());
    //     for (const auto &parent : parents)
    //     {
    //         llvm::errs() << "Parent Statement:\n";
    //         parent.print(llvm::errs(), policy);
    //         llvm::errs() << "\n";
    //     }
    //     if (!st)
    //     {
    //         llvm::outs() << "HERE \n";
    //     }
    // }

    return true; // Continue visiting other nodes
}
bool CUDA_ASTVisitor::VisitCUDAKernelCallExpr(clang::CUDAKernelCallExpr *kernelCall)
{
    targetExpressions.kernelCalls.push_back(kernelCall);
    // static int visitCount = 0;
    // llvm::errs() << "Visit Count: " << ++visitCount << "\n";
    return true;
}

bool CUDA_ASTVisitor::VisitCallExpr(clang::CallExpr *callExpr)
{
    // Change expressions that only inside the kernel function
    if (isVisitorInsideKernel)
    {
        // Check for null pointer
        if (const clang::FunctionDecl *callee = callExpr->getDirectCallee())
        {
            if (callee->getNameAsString() == "__syncthreads")
            {
                targetExpressions.syncthreadCalls.push_back(callExpr);
            }
            else if (callee->getNameAsString().substr(0, 6) == "atomic")
            {
                targetExpressions.atomicCalls.push_back(callExpr);
            }
        }
    }

    return true;
}
