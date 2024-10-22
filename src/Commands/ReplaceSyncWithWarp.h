// ReplaceSyncWithWarpCommand.h
#ifndef REPLACE_SYNC_WITH_WARP_H
#define REPLACE_SYNC_WITH_WARP_H

#include "TransformCommand.h"

class ReplaceSyncWithWarp : public TransformCommand
{
    clang::Rewriter &rewriter;
    clang::CallExpr *syncCall;
    clang::FileID mainFileID;

  public:
    ReplaceSyncWithWarp(clang::Rewriter &rewriter, clang::CallExpr *syncCall, clang::FileID mainFileID);

    void execute() override;
};

#endif