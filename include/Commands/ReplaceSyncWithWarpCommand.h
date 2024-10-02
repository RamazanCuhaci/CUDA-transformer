// ReplaceSyncWithWarpCommand.h
#ifndef REPLACESYNCWITHWARPCOMMAND_H
#define REPLACESYNCWITHWARPCOMMAND_H

#include "TransformCommand.h"

class ReplaceSyncWithWarpCommand : public TransformCommand
{
    clang::Rewriter &rewriter;
    clang::CallExpr *syncCall;

  public:
    ReplaceSyncWithWarpCommand(clang::Rewriter &rewriter, clang::CallExpr *syncCall);

    void execute() override;
};

#endif