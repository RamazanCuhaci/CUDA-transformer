set(LLVM_LINK_COMPONENTS support)
    add_clang_executable(CUDA-transformer
        src/CUDA-transformer.cpp
        src/CUDA_TransformVisitor.cpp
        src/CUDA_AnalysisVisitor.cpp
        src/CUDA_ASTConsumer.cpp
    )
    target_link_libraries(CUDA-transformer
        PRIVATE
        clangAST
        clangASTMatchers
        clangBasic
        clangFrontend
        clangSerialization
        clangTooling
        clangEdit
    )
