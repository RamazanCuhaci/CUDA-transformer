#ifndef TRANSFORMER_H
#define TRANSFORMER_H

#include <iostream>
#include <string>
#include <vector>

#include "Commands/RemoveSyncThreadCommand.h"
#include "Commands/ReplaceSyncWithWarpCommand.h"
#include "Commands/TransformCommand.h"

class Transformer
{

    std::vector<std::unique_ptr<TransformCommand>> commands; // List of commands to execute
  public:
    ~Transformer(); // Destructor to clean up commands
    void askUserForOptimization(clang::CallExpr *callExpr, clang::Rewriter &writer, const std::string &type);
    void executeCommands(); // Execute all commands
    void addCommand(std::unique_ptr<TransformCommand> command);

};

#endif