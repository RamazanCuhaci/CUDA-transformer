#include "ChooseIfElseBranch.h"

ChooseIfElseBranch::ChooseIfElseBranch(clang::Rewriter &rewriter, std::vector<clang::Stmt *> &ifElseBody,
                                       clang::ASTContext &context, std::queue<clang::SourceRange> &ifElseSourceRange,
                                       int branchNumber)
    : rewriter(rewriter), ifElseBody(ifElseBody), context(context), ifElseSourceRange(ifElseSourceRange),
      branchNumber(branchNumber)
{
}

void ChooseIfElseBranch::execute()
{

   
    std::string ifElseString;
    llvm::raw_string_ostream rso(ifElseString);
    ifElseBody[branchNumber-2]->printPretty(rso, nullptr, context.getPrintingPolicy());
    rso.flush();

    // Get the start location of the first if statement
    rewriter.ReplaceText(ifElseSourceRange.front(), ifElseString);

    rewriter.InsertTextBefore(ifElseSourceRange.front().getBegin(), "//////// CUDA-TRANSFORMER WAS HERE : Choose one if else branch\n");


    ifElseSourceRange.pop();
}