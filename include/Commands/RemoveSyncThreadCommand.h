// RemoveSyncThreadsCommand.h
#ifndef REMOVESYNCTHREADSCOMMAND_H
#define REMOVESYNCTHREADSCOMMAND_H

#include "TransformCommand.h"

class RemoveSyncThreadsCommand : public TransformCommand
{
    clang::Rewriter &rewriter;
    clang::CallExpr *syncCall;

  public:
    RemoveSyncThreadsCommand(clang::Rewriter &rewriter, clang::CallExpr *syncCall);

    void execute() override;
};

#endif
