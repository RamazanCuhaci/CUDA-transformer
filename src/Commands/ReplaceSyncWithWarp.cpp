#include "ReplaceSyncWithWarp.h"

ReplaceSyncWithWarp::ReplaceSyncWithWarp(clang::Rewriter &rewriter, clang::CallExpr *syncCall)
    : rewriter(rewriter), syncCall(syncCall)
{
}

void ReplaceSyncWithWarp::execute()
{
    rewriter.ReplaceText(syncCall->getSourceRange(), "__syncwarp()");
}
