#include "RemoveSyncThread.h"

RemoveSyncThread::RemoveSyncThread(clang::Rewriter &rewriter, clang::CallExpr *syncCall)
    : rewriter(rewriter), syncCall(syncCall)
{
}

void RemoveSyncThread::execute()
{
    // Add comment the moving area
    rewriter.ReplaceText(syncCall->getSourceRange(), "// SYNCTHREADCALL REMOVED");
}