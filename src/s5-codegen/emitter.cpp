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
    module->print(llvm::errs(), nullptr);
}

llvm::Value* Emitter::emit(pAst ast) {
    switch(ast->get_kind()) {
    case AstKind::sequence:
        return emit_sequence(ast);
    case AstKind::special_form:
        throw CompilerError();
    case AstKind::integer:
        return emit_integer(ast);
    case AstKind::identifier:
        return emit_identifier(ast);
    }
}

llvm::Value* Emitter::emit_sequence(pAst ast) {
    if(ast->retrieve_seq_s() < 1) {
        throw SyntaxError();
    }

    switch(ast->retrieve_seq_i(0)->get_kind()) {
    case AstKind::sequence:
    case AstKind::identifier:
        return emit_sequence_function_call(ast);
    case AstKind::special_form:
        return emit_sequence_special_form(ast);
    case AstKind::integer:
        throw SyntaxError();
    }
}

llvm::Value* Emitter::emit_sequence_special_form(pAst ast) {
    (void) ast;
    throw UnimplementedError();
}

llvm::Value* Emitter::emit_sequence_function_call(pAst ast) {
    if(ast->retrieve_seq_i(0)->get_kind() == AstKind::sequence) {
        throw UnimplementedError();
    }

    std::string callee_name = ast->retrieve_seq_i(0)->retrieve_symbol();
    llvm::Function *callee_f = module->getFunction(callee_name);
    if(!callee_f) {
        throw SyntaxError();
    }

    std::vector<llvm::Value *> args;
    for (size_t i = 1; i < ast->retrieve_seq_s(); i++) {
        args.push_back(emit(ast->retrieve_seq_i(i)));
        if (!args.back())
            return nullptr;
    }

    return builder.CreateCall(callee_f, args, "calltmp");
}

llvm::Value* Emitter::emit_integer(pAst ast) {
    return llvm::ConstantInt::get(context, llvm::APInt(64, ast->retrieve_int(), true));
}

llvm::Value* Emitter::emit_identifier(pAst ast) {
    llvm::Value* v = named_values[ast->retrieve_symbol()];
    if(!v) {
        throw SyntaxError();
    }
    return v;
}
