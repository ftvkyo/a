#pragma once

#include <llvm/IR/IRBuilder.h>

#include "a/s2-parsing/ast.hpp"


class Emitter {
public:

    Emitter();

    void feed(pAst ast);

    void print();

private:

    void populate_builtins();

    llvm::Value* emit(pAst ast);

    llvm::Value* emit_sf(pAst ast);

    llvm::Value* emit_call(pAst ast);

    llvm::Value* emit_integer(pAst ast);

    llvm::Value* emit_identifier(pAst ast);

    llvm::Value* make_main(llvm::Value* callee);

    bool fed = false;

    llvm::LLVMContext context;
    llvm::IRBuilder<> builder;
    std::unique_ptr<llvm::Module> module;
    std::map<std::string, llvm::Value*> named_values;
};
