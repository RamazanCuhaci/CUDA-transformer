#include "TransformCommand.h"
#include "clang/Lex/Lexer.h"
#include <queue>


class RemoveIfElseBranches : public TransformCommand
{
    clang::Rewriter &rewriter;
    std::vector<clang::Stmt *> ifElseBody;
    clang::ASTContext &context;
    std::queue<clang::SourceRange> &ifElseSourceRange;

  public:
    RemoveIfElseBranches(clang::Rewriter &rewriter, std::vector<clang::Stmt *> &ifElseBody, clang::ASTContext &context,
                         std::queue<clang::SourceRange> &ifElseSourceRange);

    void execute() override;
};