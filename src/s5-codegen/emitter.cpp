#include "a/prelude.hpp"


Emitter::Emitter() :
    builder(context)
{}

llvm::Value* Emitter::log_error(std::string err) {
    std::cerr << err;
    return nullptr;
}
