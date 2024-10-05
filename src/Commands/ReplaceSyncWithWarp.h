// ReplaceSyncWithWarpCommand.h
#ifndef REPLACE_SYNC_WITH_WARP_H
#define REPLACE_SYNC_WITH_WArP_H

#include "TransformCommand.h"

class ReplaceSyncWithWarp : public TransformCommand
{
    clang::Rewriter &rewriter;
    clang::CallExpr *syncCall;

  public:
    ReplaceSyncWithWarp(clang::Rewriter &rewriter, clang::CallExpr *syncCall);

    void execute() override;
};

#endif