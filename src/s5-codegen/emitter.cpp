#include <sstream>

#include <llvm/IR/Verifier.h>

#include "a/prelude.hpp"


Emitter::Emitter() :
    context("a")
{}

void Emitter::feed(pAst ast) {
    if(fed) {
        throw CompilerError("Feeding an emitter twice.");
    }
    populate_builtins();
    llvm::Value* top_block = emit(ast);
    llvm::Value* main = make_main(top_block);
    (void) main;
    fed = true;
}

void Emitter::print() {
    context.m->print(llvm::outs(), nullptr);
}

void Emitter::populate_builtins() {
    std::vector<llvm::Type *> binary_func_args(2, llvm::Type::getInt64Ty(context.c));
    llvm::FunctionType *binary_func_type = llvm::FunctionType::get(llvm::Type::getInt64Ty(context.c), binary_func_args, false);

    llvm::Function *f_add = llvm::Function::Create(binary_func_type, llvm::Function::ExternalLinkage, "f_add", context.m.get());

    auto f_add_arg_it = f_add->arg_begin();
    llvm::Value* arg0 = f_add_arg_it++;
    arg0->setName("arg0");
    llvm::Value* arg1 = f_add_arg_it++;
    arg1->setName("arg1");

    llvm::BasicBlock *bb_add = llvm::BasicBlock::Create(context.c, "entry", f_add);
    context.b.SetInsertPoint(bb_add);

    llvm::Value* add = context.b.CreateAdd(arg0, arg1);

    context.b.CreateRet(add);
}

llvm::Value* Emitter::emit(pAst ast) {
    switch(ast->get_kind()) {
    case AstKind::function:
        return emit_call(ast);
    case AstKind::special_form:
        return emit_sf(ast);
    case AstKind::integer:
        return emit_integer(ast);
    case AstKind::identifier:
        return emit_identifier(ast);
    default:
        throw CompilerError("Emitter::emit got unexpected input.");
    }
}

llvm::Value* Emitter::emit_sf(pAst ast) {
    if(ast->retrieve_seq_i(0)->retrieve_symbol() == "@block") {
        std::vector<llvm::Type *> ints(0);
        llvm::FunctionType *ft = llvm::FunctionType::get(llvm::Type::getInt64Ty(context.c), ints, false);
        llvm::Function *f = llvm::Function::Create(ft, llvm::Function::ExternalLinkage, "__anon_expr", context.m.get());

        llvm::BasicBlock *bb = llvm::BasicBlock::Create(context.c, "entry", f);
        context.b.SetInsertPoint(bb);

        auto last_expr = ast->retrieve_seq_i(ast->retrieve_seq_s() - 1);
        if (llvm::Value* retval = emit(last_expr)) {
            // Finish off the function.
            context.b.CreateRet(retval);

            // Validate the generated code, checking for consistency.
            llvm::verifyFunction(*f);

            return f;
        }

        // Error reading body, remove function.
        f->eraseFromParent();
        return nullptr;
    } else {
        throw UnimplementedError("Special forms other than @block are not supported yet.");
    }
}

llvm::Value* Emitter::emit_call(pAst ast) {
    if(ast->retrieve_seq_i(0)->get_kind() != AstKind::identifier) {
        throw UnimplementedError("Anonymous functions are not supported yet.");
    }

    std::string callee_name = ast->retrieve_seq_i(0)->retrieve_symbol();
    if(callee_name == "+") {
        callee_name = "f_add";
    }

    llvm::Function *callee_f = context.m->getFunction(callee_name);
    if(!callee_f) {
        throw SyntaxError("Function does not exist.");
    }

    std::vector<llvm::Value *> args;
    for (size_t i = 1; i < ast->retrieve_seq_s(); i++) {
        args.push_back(emit(ast->retrieve_seq_i(i)));
        if (!args.back()) {
            return nullptr;
        }
    }

    return context.b.CreateCall(callee_f, args, "calltmp");
}

llvm::Value* Emitter::emit_integer(pAst ast) {
    return llvm::ConstantInt::get(context.c, llvm::APInt(64, ast->retrieve_int(), true));
}

llvm::Value* Emitter::emit_identifier(pAst ast) {
    llvm::Value* v = context.nv[ast->retrieve_symbol()];
    if(!v) {
        throw SyntaxError("Identifier does not exist.");
    }
    return v;
}

llvm::Value* Emitter::make_main(llvm::Value* callee) {
    std::vector<llvm::Type *> args;
    llvm::FunctionType *ft = llvm::FunctionType::get(llvm::Type::getInt64Ty(context.c), args, false);
    llvm::Function *f = llvm::Function::Create(ft, llvm::Function::ExternalLinkage, "main", context.m.get());

    llvm::BasicBlock *bb = llvm::BasicBlock::Create(context.c, "entry", f);
    context.b.SetInsertPoint(bb);

    // Record the function arguments in the NamedValues map.
    context.nv.clear();

    std::vector<llvm::Type *> callee_args(1, llvm::Type::getInt64Ty(context.c));
    llvm::FunctionType *callee_type = llvm::FunctionType::get(llvm::Type::getInt64Ty(context.c), args, false);
    llvm::FunctionCallee c(callee_type, callee);
    llvm::Value* call = context.b.CreateCall(c);

    // Finish off the function.
    context.b.CreateRet(call);

    // Validate the generated code, checking for consistency.
    llvm::verifyFunction(*f);

    return f;
}
