#pragma once

#include <llvm/IR/IRBuilder.h>


struct Context {
    Context(std::string mod);

    llvm::LLVMContext c; // Context
    llvm::IRBuilder<> b; // Builder
    std::unique_ptr<llvm::Module> m; // Module
    std::map<std::string, llvm::Value*> nv; //Named values
};
