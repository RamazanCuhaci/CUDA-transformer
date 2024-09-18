set(LLVM_LINK_COMPONENTS support)
add_clang_executable(CUDA-transformer
    CUDA-transformer.cpp
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
