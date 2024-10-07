#include "TransformCommand.h"

class RemoveIfElseBranches : public TransformCommand
{
    clang::Rewriter &rewriter;
    std::vector<clang::Stmt *> ifElseBody;

  public:
    RemoveIfElseBranches(clang::Rewriter &rewriter, std::vector<clang::Stmt *> ifElseBody);

    void execute() override;
};