#pragma once

#include <llvm/IR/IRBuilder.h>

#include "a/s2-parsing/ast.hpp"


class Emitter {
public:

    Emitter();

    llvm::Value* emit(pAst &ast);

private:

    llvm::Value* log_error(std::string err);

    llvm::LLVMContext context;
    llvm::IRBuilder<> builder;
    std::unique_ptr<llvm::Module> module;
    std::map<std::string, llvm::Value *> named_values;
};
