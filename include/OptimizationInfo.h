#ifndef OPTIMIZATION_INFO_H
#define OPTIMIZATION_INFO_H

#include "clang/AST/Expr.h"
#include <vector>

class OptimizationInfo 
{
    
public:
    std::vector<clang::CallExpr*> syncthreadCalls;
    std::vector<clang::CallExpr*> atomicCalls;
};


#endif