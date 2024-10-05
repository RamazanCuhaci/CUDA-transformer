#ifndef TRANSFORMER_H
#define TRANSFORMER_H

#include <iostream>
#include <string>
#include <vector>

#include "Commands/RemoveSyncThread.h"
#include "Commands/ReplaceSyncWithWarp.h"
#include "Commands/ReplaceAtomicWithBlock.h"
#include "Commands/TransformCommand.h"

class Transformer
{

    
    std::vector<std::unique_ptr<TransformCommand>> commands; // List of commands to execute

    void displayWrongChoiceError();
  
  public:
    Transformer() = default;
    ~Transformer() = default; // Destructor to clean up commands
    void executeCommands(); // Execute all commands
    void addCommand(std::unique_ptr<TransformCommand> command);
    
    /* Analyze and ask user to choice for optimization
    */
    void analyzeSyncthread(clang::CallExpr *callExpr, clang::Rewriter &writer);
    void analyzeAtomicCalls(clang::CallExpr *callExpr, clang::Rewriter &writer, clang::ASTContext &context);

};

#endif