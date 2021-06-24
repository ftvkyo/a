#pragma once

#include <llvm/IR/IRBuilder.h>

#include "a/s2-parsing/ast.hpp"


class Emitter {
public:

    Emitter();

    void feed(pAst ast);

    void print();

private:

    llvm::Value* emit(pAst ast);

    llvm::Value* emit_sequence(pAst ast);

    llvm::Value* emit_sequence_special_form(pAst ast);

    llvm::Value* emit_sequence_function_call(pAst ast);

    llvm::Value* emit_integer(pAst ast);

    llvm::Value* emit_identifier(pAst ast);

    llvm::LLVMContext context;
    llvm::IRBuilder<> builder;
    std::unique_ptr<llvm::Module> module;
    std::map<std::string, llvm::Value*> named_values;
};
