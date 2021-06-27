#include <sstream>

#include <llvm/IR/Verifier.h>

#include "a/prelude.hpp"


Emitter::Emitter() :
    builder(context),
    module(new llvm::Module("a", context))
{}

void Emitter::feed(pAst ast) {
    llvm::Value* v = emit(ast);
    (void) v;
}

void Emitter::print() {
    module->print(llvm::outs(), nullptr);
}

llvm::Value* Emitter::emit(pAst ast) {
    switch(ast->get_kind()) {
    case AstKind::function:
        return emit_function(ast);
    case AstKind::special_form:
        return emit_special_form(ast);
    case AstKind::integer:
        return emit_integer(ast);
    case AstKind::identifier:
        return emit_identifier(ast);
    default:
        throw CompilerError("Emitter::emit got unexpected input.");
    }
}

llvm::Value* Emitter::emit_special_form(pAst ast) {
    if(ast->retrieve_seq_i(0)->retrieve_symbol() == "@block") {
        std::vector<llvm::Type *> ints(1, llvm::Type::getInt64Ty(context));
        llvm::FunctionType *ft = llvm::FunctionType::get(llvm::Type::getInt64Ty(context), ints, false);
        llvm::Function *f = llvm::Function::Create(ft, llvm::Function::ExternalLinkage, "__anon_expr", module.get());

        // Set names for all arguments.
        unsigned i = 0;
        for (auto &arg : f->args()) {
            std::stringstream ss;
            ss << "arg" << i;
            arg.setName(ss.str());
        }

        llvm::BasicBlock *bb = llvm::BasicBlock::Create(context, "entry", f);
        builder.SetInsertPoint(bb);

        // Record the function arguments in the NamedValues map.
        named_values.clear();
        for (auto &arg : f->args()) {
            named_values[std::string(arg.getName())] = &arg;
        }

        if (llvm::Value* retval = emit(ast->retrieve_seq_i(1))) {
            // Finish off the function.
            builder.CreateRet(retval);

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

llvm::Value* Emitter::emit_function(pAst ast) {
    if(ast->retrieve_seq_i(0)->get_kind() != AstKind::identifier) {
        throw UnimplementedError("Anonymous functions are not supported yet.");
    }

    std::string callee_name = ast->retrieve_seq_i(0)->retrieve_symbol();
    llvm::Function *callee_f = module->getFunction(callee_name);
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

    return builder.CreateCall(callee_f, args, "calltmp");
}

llvm::Value* Emitter::emit_integer(pAst ast) {
    return llvm::ConstantInt::get(context, llvm::APInt(64, ast->retrieve_int(), true));
}

llvm::Value* Emitter::emit_identifier(pAst ast) {
    llvm::Value* v = named_values[ast->retrieve_symbol()];
    if(!v) {
        throw SyntaxError("Identifier does not exist.");
    }
    return v;
}
