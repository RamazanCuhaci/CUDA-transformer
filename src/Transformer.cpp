#include "Transformer.h"

void Transformer::addCommand(std::unique_ptr<TransformCommand> command)
{
    commands.push_back(std::move(command));
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

void Transformer::analyzeSyncthread(clang::CallExpr *callExpr, clang::Rewriter &rewriter, clang::ASTContext &context)
{

    int choice{-1};

    // If there is no valid choice ask again
    while (choice != 0 && choice != 1 && choice != 2 && choice != 3 && choice != 4)
    {
        llvm::outs() << "Optimization options for __syncthreads:\n"
                     << "0. Do nothing\n"
                     << "1. Replace with __syncwarp()\n"
                     << "2. Remove __syncthreads\n"
                     << "3. Synchronize only 4 threads in the group\n"
                     << "4. Synchronize only active threads"
                     << "Enter choice: ";

        std::cin >> choice;

        if (choice == 0)
        {
            return;
        }
        else if (choice == 1)
        {
            addCommand(std::make_unique<ReplaceSyncWithWarp>(rewriter, callExpr));
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
}

void Transformer::analyzeAtomicCalls(clang::CallExpr *callExpr, clang::Rewriter &rewriter, clang::ASTContext &context)
{

    int choice{-1};

    // If there is no valid choice ask again
    while (choice != 0 && choice != 1 && choice != 2)
    {

        llvm::outs() << "Optimization options for atomic function:\n"
                     << "0. Do nothing\n"
                     << "1. Replace with atomic block\n"
                     << "2. Replace with direct operation\n"
                     << "Enter choice: ";
        std::cin >> choice;

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
}

void Transformer::analyzeKernelCall(clang::CUDAKernelCallExpr *callExpr, clang::Rewriter &rewriter,
                                    clang::ASTContext &context)
{

    int choice{-1};

    // If there is no valid choice ask again
    while (choice != 0 && choice != 1 && choice != 2)
    {

        llvm::outs() << "Optimization options for kernel call:\n"
                     << "0. Do nothing\n"
                     << "1. Block reduction (type '1 deductionRate')"
                     << "Enter choice: ";

        int blockReductionRate{};
        int threadReductionRate{};
        std::cin >> choice >> blockReductionRate >> threadReductionRate;

        if (choice == 0)
        {
            return;
        }
        else if (choice == 1)
        {
            addCommand(std::make_unique<KernelCallReduction>(rewriter, callExpr, context, blockReductionRate,
                                                             threadReductionRate));
        }
        else
        {
            displayWrongChoiceError();
        }
    }
}

void Transformer::analyzeIfElse(std::vector<clang::Stmt *> &ifElseBody, clang::Rewriter &writer,
                                clang::ASTContext &context, std::queue<clang::SourceRange> &ifElseSourceRange)
{

    int choice{-1};

    // If there is no valid choice ask again
    while (choice != 0 && choice != 1 && choice != 2)
    {

        llvm::outs() << "If else branches found. Branch count: " << ifElseBody.size() << "\n"
                     << "Optimization options for if-else statement:\n"
                     << "0. Do nothing\n"
                     << "1. Delete all\n"
                     << "2. Replace with choosen branch (type '2 branchNumber')"
                     << "Enter choice: ";
        std::cin >> choice;

        if (choice == 0)
        {
            return;
        }
        else if (choice == 1)
        {
            addCommand(std::make_unique<RemoveIfElseBranches>(writer, ifElseBody, context, ifElseSourceRange));
        }
        else if (choice == 2)
        {
            int branchNumber{};
            llvm::outs() << "Enter branch number: ";
            std::cin >> branchNumber;
            addCommand(std::make_unique<ChooseIfElseBranch>(writer, ifElseBody, context, ifElseSourceRange, branchNumber));
        }
        else
        {
            displayWrongChoiceError();
        }
    }
}

void Transformer::analyzeDoubles(clang::TypeLoc typeLoc, clang::Rewriter &writer)
{

    int choice{-1};

    // If there is no valid choice ask again
    while (choice != 0 && choice != 1)
    {

        llvm::outs() << "Optimization options for double type:\n"
                     << "0. Do nothing\n"
                     << "1. Replace with float\n"
                     << "Enter choice: ";
        std::cin >> choice;

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

}
