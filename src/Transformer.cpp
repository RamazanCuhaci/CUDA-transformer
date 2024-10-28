#include "Transformer.h"

void Transformer::addCommand(std::unique_ptr<TransformCommand> command)
{
    commands.push_back(std::move(command));
}

void Transformer::getOptimizationChoices(std::string& choices)
{

    for (char c : choices)
    {
        if (isdigit(c))
        {
            int digit = c - '0';
            choiceQueue.push(digit);
        }
    }
}
void Transformer::displayWrongChoiceError()
{
    llvm::outs() << "######################################\n ERROR : Invalid Choice ! Choose again "
                    "\n######################################\n";
}

void Transformer::executeCommands()
{
    for (const auto &command : commands)
    {
        command->execute();
    }
}

void Transformer::analyzeSyncthread(clang::CallExpr *callExpr, clang::Rewriter &rewriter, clang::ASTContext &context, clang::FileID mainFileID)
{

    int choice = choiceQueue.front();
    choiceQueue.pop();

    if (choice == 0)
    {
        return;
    }
    else if (choice == 1)
    {
        addCommand(std::make_unique<ReplaceSyncWithWarp>(rewriter, callExpr, mainFileID));
    }
    else if (choice == 2)
    {
        addCommand(std::make_unique<RemoveSyncThread>(rewriter, callExpr));
    }
    else if (choice == 3)
    {
        addCommand(std::make_unique<ReplaceSyncThreadWithTile>(rewriter, callExpr, context));
    }
    else if (choice == 4)
    {
        addCommand(std::make_unique<ReplaceSyncThreadWithActive>(rewriter, callExpr, context));
    }
    else
    {
        displayWrongChoiceError();
    }
}

void Transformer::analyzeAtomicCalls(clang::CallExpr *callExpr, clang::Rewriter &rewriter, clang::ASTContext &context)
{

    int choice = choiceQueue.front();
    choiceQueue.pop();

    if (choice == 0)
    {
        return;
    }
    else if (choice == 1)
    {
        addCommand(std::make_unique<ReplaceAtomicWithBlock>(rewriter, callExpr, context));
    }
    else if (choice == 2)
    {
        addCommand(std::make_unique<ReplaceAtomicWithDirect>(rewriter, callExpr, context));
    }
    else
    {
        displayWrongChoiceError();
    }
}

void Transformer::analyzeKernelCall(clang::CUDAKernelCallExpr *callExpr, clang::Rewriter &rewriter,
                                    clang::ASTContext &context)
{

    int blockReductionNumber{};
    int threadReductionNumber{};

    int blockChoice = choiceQueue.front();
    choiceQueue.pop();

    int threadChoice = choiceQueue.front();
    choiceQueue.pop();

    if (blockChoice == 0  && threadChoice == 0)
    {
        return;
    }
    if (blockChoice == 1)
    {
        blockReductionNumber = 1;
    }
    else if (blockChoice == 2)
    {
        blockReductionNumber = 2;
    }

    if (threadChoice == 1)
    {
        threadReductionNumber = 32;
    }
    else if (threadChoice == 2)
    {
        threadReductionNumber = 64;
    }

    addCommand(std::make_unique<KernelCallReduction>(rewriter, callExpr, context, blockReductionNumber,
                                                     threadReductionNumber));
}

void Transformer::analyzeIfElse(std::vector<clang::Stmt *> &ifElseBody, clang::Rewriter &writer,
                                clang::ASTContext &context, std::queue<clang::SourceRange> &ifElseSourceRange)
{

    int choice = choiceQueue.front();
    choiceQueue.pop();
    
    if (choice == 0)
    {
        return;
    }
    else if (choice == 1)
    {
        addCommand(std::make_unique<RemoveIfElseBranches>(writer, ifElseBody, context, ifElseSourceRange));
    }
    else if (choice == 3)
    {
        addCommand(std::make_unique<ChooseIfElseBranch>(writer, ifElseBody, context, ifElseSourceRange, choice));
    }
    else if (choice == 2)
    {
        addCommand(std::make_unique<ChooseIfElseBranch>(writer, ifElseBody, context, ifElseSourceRange, choice));
    }
    else if (choice == 4)
    {
        addCommand(std::make_unique<ChooseIfElseBranch>(writer, ifElseBody, context, ifElseSourceRange, choice));
    }
    
}

void Transformer::analyzeDoubles(clang::TypeLoc typeLoc, clang::Rewriter &writer)
{

    int choice = choiceQueue.front();
    choiceQueue.pop();

    if (choice == 0)
    {
        return;
    }
    else if (choice == 1)
    {
        addCommand(std::make_unique<ReplaceDoubleWithFloat>(writer, typeLoc));
    }
    else
    {
        displayWrongChoiceError();
    }
}

void Transformer::analyzeForStmt(clang::ForStmt *forStmt, clang::Rewriter &writer, clang::ASTContext &context)
{
    int choice = choiceQueue.front();
    choiceQueue.pop();

    if (choice == 0)
    {
        return;
    }
    else if (choice == 1)
    {
        float perforationRate = 0.05f;
        addCommand(std::make_unique<LoopPerforation>(writer, forStmt, context, 1-perforationRate));
    }
    else if (choice == 2)
    {
        float perforationRate = 0.1f;
        addCommand(std::make_unique<LoopPerforation>(writer, forStmt, context, 1-perforationRate));
    }
    else if (choice == 3)
    {
        float perforationRate = 0.15f;
        addCommand(std::make_unique<LoopPerforation>(writer, forStmt, context, 1-perforationRate));
    }
    else if(choice == 4)
    {
        float perforationRate = 0.2f;
        addCommand(std::make_unique<LoopPerforation>(writer, forStmt, context, 1-perforationRate));
    }
    else
    {
        displayWrongChoiceError();
    }
}
