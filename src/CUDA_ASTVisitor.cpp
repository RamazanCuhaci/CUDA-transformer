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
                    // Get the if-else source range
                    clang::SourceLocation start = ifStmt->getIfLoc();

                    if (processedStatements.find(ifStmt) != processedStatements.end())
                    {
                        continue; // Skip already processed statements
                    }

                    std::vector<clang::Stmt *> currentBranch;
                    currentBranch.push_back(ifStmt->getThen());
                    processedStatements.insert(ifStmt->getThen());

                    clang::Stmt *elseStmt = ifStmt->getElse();
                    clang::SourceLocation end = elseStmt->getEndLoc();

                    targetExpressions.ifElseSourceRange.push(clang::SourceRange(start, end));
                    while (elseStmt)
                    {
                        if (auto *elseIfStmt = llvm::dyn_cast<clang::IfStmt>(elseStmt))
                        {
                            currentBranch.push_back(elseIfStmt->getThen());
                            processedStatements.insert(elseIfStmt->getThen());
                            elseStmt = elseIfStmt->getElse();
                            end = elseStmt->getEndLoc();
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

bool CUDA_ASTVisitor::VisitTypeLoc(clang::TypeLoc typeLoc)
{
    if (isVisitorInsideKernel)
    {
        clang::QualType type = typeLoc.getType();

        if (type->isSpecificBuiltinType(clang::BuiltinType::Double))
        {
            targetExpressions.doubles.push_back(typeLoc);
        }
    }

    return true;
}
