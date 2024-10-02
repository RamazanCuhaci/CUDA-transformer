#include "../include/Transformer.h"


void Transformer::addCommand(std::unique_ptr<TransformCommand> command)
{
    commands.push_back(std::move(command));
}

void Transformer::executeCommands()
{
    for (const auto &command : commands)
    {
        command->execute();
    }
}

// Ask user for optimization choice based on the expression type (e.g., __syncthreads, atomic, kernel)
void Transformer::askUserForOptimization(clang::CallExpr *callExpr, clang::Rewriter &rewriter, const std::string &type)
{
    int choice;

    if (type == "__syncthreads")
    {
        llvm::outs() << "Optimization options for __syncthreads:\n"
                     << "1. Replace with __syncwarp()\n"
                     << "2. Remove __syncthreads\n"
                     << "3. Do nothing\n"
                     << "Enter choice: ";
    }
    else if (type == "atomic")
    {
        llvm::outs() << "Optimization options for atomic function:\n"
                     << "1. Replace with atomic block\n"
                     << "2. Replace with direct operation\n"
                     << "3. Do nothing\n"
                     << "Enter choice: ";
    }
    else if (type == "kernel")
    {
        llvm::outs() << "Optimization options for CUDA kernel call:\n"
                     << "1. Modify configuration\n"
                     << "2. Do nothing\n"
                     << "Enter choice: ";
    }

    std::cin >> choice;

    if (type == "__syncthreads")
    {
        if (choice == 1)
        {
            addCommand(std::make_unique<ReplaceSyncWithWarpCommand>(rewriter, callExpr));
        }
        else if (choice == 2)
        {
            addCommand(std::make_unique<RemoveSyncThreadsCommand>(rewriter, callExpr));
        }
    }
    // Handle atomic and kernel optimizations similarly...
}
