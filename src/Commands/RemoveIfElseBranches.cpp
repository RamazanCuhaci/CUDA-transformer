#include "RemoveIfElseBranches.h"

RemoveIfElseBranches::RemoveIfElseBranches(clang::Rewriter &rewriter, std::vector<clang::Stmt *> &ifElseBody,
                                           clang::ASTContext &context,
                                           std::queue<clang::SourceRange> &ifElseSourceRange)
    : rewriter(rewriter), ifElseBody(ifElseBody), context(context), ifElseSourceRange(ifElseSourceRange)
{
}

void RemoveIfElseBranches::execute()
{

    // clang::PrintingPolicy policy(context.getLangOpts());
    // llvm::errs() << ifElseBody.size() << "\n";
    // for (const auto &stmt : ifElseBody)
    // {

    //     stmt->printPretty(llvm::errs(), nullptr, policy);
    //     llvm::errs() << "*********************************************************\n";
    // }

    
    // Get the start location of the first if statement
    rewriter.RemoveText(ifElseSourceRange.front());
    

    rewriter.InsertTextBefore(ifElseSourceRange.front().getBegin(), "//////// CUDA-TRANSFORMER WAS HERE : If-Else Branches Removed\n");

    // Remove the source range from the queue
    ifElseSourceRange.pop();
}