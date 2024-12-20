
#include "CUDA_ASTConsumer.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendAction.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/Support/CommandLine.h"

// Declare tool options
static llvm::cl::OptionCategory MyToolCategory("my-tool options");

llvm::cl::opt<std::string> OutputFilename("o", llvm::cl::desc("Specify output filename"), llvm::cl::value_desc("filename"));

llvm::cl::opt<bool> AnalyzerMode("analyze", llvm::cl::desc("Analyze the code and print optimization possibilities"));
llvm::cl::opt<std::string> OptChoices("choices", llvm::cl::desc("Apply the optimizations"));

class CUDA_FrontendAction : public clang::ASTFrontendAction
{

    clang::Rewriter TheRewriter;
    Expressions targetExpressions;
    Transformer transformer;

    // The common entry point for the tool
  public:
    std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(clang::CompilerInstance &Compiler,
                                                          llvm::StringRef InFile) override
    {
        TheRewriter.setSourceMgr(Compiler.getSourceManager(), Compiler.getLangOpts());
        return std::make_unique<CUDA_ASTConsumer>(&Compiler.getASTContext(), TheRewriter, targetExpressions,
                                                  transformer, AnalyzerMode, OptChoices);
    }

    void EndSourceFileAction() override
    {
        // Get the source manager
        clang::SourceManager &SM = TheRewriter.getSourceMgr();
        clang::FileID mainFileID = SM.getMainFileID();

        // Create the new filename with "new_" prefix
        std::string newFileName = OutputFilename;

        // Open a file stream for the new file
        std::error_code EC;
        llvm::raw_fd_ostream outFile(newFileName, EC, llvm::sys::fs::OF_None);

        // Check for file opening errors
        if (EC)
        {
            llvm::errs() << "Error opening file " << newFileName << ": " << EC.message() << "\n";
            return;
        }

        // Write the modified source code to the new file
        TheRewriter.getEditBuffer(mainFileID).write(outFile);

        // Close the file stream
        outFile.close();
    }
};

int main(int argc, const char **argv)
{

    // Create parser for command line options
    auto ExpectedParser = clang::tooling::CommonOptionsParser::create(argc, argv, MyToolCategory);

    if (!ExpectedParser)
    {
        llvm::errs() << ExpectedParser.takeError();
        return 1;
    }

    // Get command line options
    clang::tooling::CommonOptionsParser &op = ExpectedParser.get(); // Use the fully qualified name
    clang::tooling::ClangTool Tool(op.getCompilations(), op.getSourcePathList());

    // Create factory for front end actions and run
    // Front end action class has ASTConsumer and ASTConsumer has ASTVisitor
    // Front end action is the main entry point for the tool.
    auto Factory = clang::tooling::newFrontendActionFactory<CUDA_FrontendAction>();
    int Result = Tool.run(Factory.get());

    if (Result != 0)
    {
        llvm::errs() << "Error occurred while running the tool.\n";
        return Result;
    }
    return 0;
};
