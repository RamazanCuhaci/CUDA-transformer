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

void Transformer::analyzeSyncthread(clang::CallExpr *callExpr, clang::Rewriter &rewriter)
{

    int choice{-1};

    // If there is no valid choice ask again
    while (choice != 0 && choice != 1 && choice != 2)
    {
        llvm::outs() << "Optimization options for __syncthreads:\n"
                     << "0. Do nothing\n"
                     << "1. Replace with __syncwarp()\n"
                     << "2. Remove __syncthreads\n"
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
            // addCommand(std::make_unique<ReplaceWithBlock>(rewriter, callExpr));
        }
        else
        {
            displayWrongChoiceError();
        }
    }
}
