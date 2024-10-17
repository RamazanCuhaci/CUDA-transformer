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

   

    clang::Expr *condition = forStmt->getInc();
   
    
    clang::SourceRange conditionRange = condition->getSourceRange();
    
    std::string appendedString = " * (" + std::to_string(perforationRate) + " )";

    rewriter.ReplaceText(conditionRange, appendedString);
    
    rewriter.InsertTextBefore(forStmt->getBeginLoc(), "//////// CUDA-TRANSFORMER WAS HERE : Loop Perforation\n");
}