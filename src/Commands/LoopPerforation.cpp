#include "LoopPerforation.h"
#include "clang/Lex/Lexer.h"

LoopPerforation::LoopPerforation(clang::Rewriter &rewriter, clang::ForStmt *forStmt, clang::ASTContext &context, float perforationRate)
    : rewriter(rewriter), forStmt(forStmt), context(context), perforationRate(perforationRate)
{
}

void LoopPerforation::printForStmt()
{
    clang::PrintingPolicy policy(context.getLangOpts());
    llvm::errs() << "For loop before transformation:\n";
    forStmt->printPretty(llvm::errs(), nullptr, policy);
    llvm::errs() << "*********************************************************\n";
}

void LoopPerforation::execute()
{

    if (!forStmt)
    {
        llvm::errs() << "For statement is null, skipping insertion." << "\n";
        return;
    }

    clang::Expr *condition = forStmt->getCond();
    if (!condition)
    {
        llvm::errs() << "For statement condition is null, skipping insertion." << "\n";
        return;
    }
    
    clang::SourceRange conditionRange = condition->getSourceRange();

    clang::SourceLocation endLoc = clang::Lexer::getLocForEndOfToken(conditionRange.getEnd(), 0,
                                                                     context.getSourceManager(), context.getLangOpts());

    // Debugging: Print the end location
    llvm::errs() << "End location: " << endLoc.printToString(context.getSourceManager()) << "\n";

    if (endLoc.isInvalid())
    {
        llvm::errs() << "Invalid end location, skipping insertion." << "\n";
        return;
    }
    
    std::string appendedString = " * (" + std::to_string(perforationRate) + " )";

    rewriter.InsertText(endLoc, appendedString);
    
    rewriter.InsertTextBefore(forStmt->getBeginLoc(), "//////// CUDA-TRANSFORMER WAS HERE : Loop Perforation\n");
}