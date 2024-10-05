#include "ReplaceAtomicWithBlock.h"

ReplaceAtomicWithBlock::ReplaceAtomicWithBlock(clang::Rewriter &rewriter, clang::CallExpr *atomicCall,
                                               clang::ASTContext &context)
    : rewriter(rewriter), atomicCall(atomicCall), context(context)
{
}

void ReplaceAtomicWithBlock::execute()
{

    std::string newText = atomicCall->getDirectCallee()->getNameAsString() + "_block(";

    // Iterate over the arguments and append them to the newText string
    for (size_t i = 0; i < atomicCall->getNumArgs(); ++i)
    {
        std::string argText =
            clang::Lexer::getSourceText(clang::CharSourceRange::getTokenRange(atomicCall->getArg(i)->getSourceRange()),
                                        context.getSourceManager(), context.getLangOpts())
                .str();
        if (i > 0)
        {
            newText += ", ";
        }
        newText += argText;
    }
    newText += ")";

    // Capture the end location of the atomic call's statement
    clang::SourceLocation endLocation = atomicCall->getEndLoc().getLocWithOffset(1);

    // Replace the entire expression with the new function call
    clang::SourceRange range = atomicCall->getSourceRange();
    rewriter.ReplaceText(range, newText);

    // Add a comment after the full statement, after the semicolon
    rewriter.InsertTextAfterToken(endLocation, " // Changed to atomic block function");
}