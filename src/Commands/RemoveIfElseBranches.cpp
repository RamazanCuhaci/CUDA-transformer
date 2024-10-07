#include "RemoveIfElseBranches.h"

RemoveIfElseBranches::RemoveIfElseBranches(clang::Rewriter &rewriter, std::vector<clang::Stmt *> ifElseBody)
    : rewriter(rewriter), ifElseBody(ifElseBody)
{
}

void RemoveIfElseBranches::execute()
{

    clang::SourceLocation start = ifElseBody[0]->getBeginLoc();
    clang::SourceLocation end = ifElseBody[ifElseBody.size() - 1]->getEndLoc();

    rewriter.RemoveText(clang::SourceRange(start, end));

}