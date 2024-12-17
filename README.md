# CUDATransformer

This tool is used to modify and transform CUDA source codes. This README file provides instructions on how to install, configure, and use the tool.

## Installation

1. To use this tool, first download and compile the [LLVM project](https://llvm.org/).

2. While compiling the LLVM project, clone a repo under the `clang-tools-extra` directory.

3. Add the following line to the `CMakeLists.txt` file under the `clang-tools-extra` directory:
    ```cmake
    add_subdirectory(CUDAIntegratedTransformerTool)
    ```

6. Navigate to the build directory of the LLVM project and create make files with cmake. Enable clang-tools-extra option. You dont't need for this every changes in the code. You can just do step 7 for any source code changes after the first installation.
    ```bash
    sudo cmake -DLLVM_ENABLE_PROJECTS="clang-tools-extra;clang" -DCMAKE_BUILD_TYPE=Release -G "Unix Makefiles" ../llvm
    ```

6. Finally, navigate to the build directory of the LLVM project and compile the tool by running the following commands. "-j 16 " for building with 16 core. :
    ```bash
    sudo make -j 16 install CUDAIntegratedTransformerTool
    ```

## Samples

Samples created for [PolyBench-ACC](https://github.com/cavazos-lab/PolyBench-ACC) benchmark programs. Samples link [samples](https://drive.google.com/drive/folders/1aiotg3w3qJnpWaZLr2So1B73umVrQbQR?usp=sharing
))
