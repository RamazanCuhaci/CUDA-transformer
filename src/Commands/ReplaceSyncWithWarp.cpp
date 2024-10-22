#include "ReplaceSyncWithWarp.h"

ReplaceSyncWithWarp::ReplaceSyncWithWarp(clang::Rewriter &rewriter, clang::CallExpr *syncCall, clang::FileID mainFileID)
    : rewriter(rewriter), syncCall(syncCall), mainFileID(mainFileID)
{
}

void ReplaceSyncWithWarp::execute()
{

    rewriter.ReplaceText(syncCall->getSourceRange(), "__syncwarp()");
    
    // Add a comment after the full statement, after the semicolon
    rewriter.InsertTextAfterToken(syncCall->getEndLoc().getLocWithOffset(1),
                                  "\t//////// CUDA-TRANSFORMER WAS HERE : __syncthreads() replaced with __syncwarps()");
}
