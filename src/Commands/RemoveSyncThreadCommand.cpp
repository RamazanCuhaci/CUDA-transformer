#include "../../include/Commands/RemoveSyncThreadCommand.h"

RemoveSyncThreadsCommand::RemoveSyncThreadsCommand(clang::Rewriter &rewriter, clang::CallExpr *syncCall)
    : rewriter(rewriter), syncCall(syncCall)
{
}

void RemoveSyncThreadsCommand::execute()
{
    rewriter.RemoveText(syncCall->getSourceRange());
}