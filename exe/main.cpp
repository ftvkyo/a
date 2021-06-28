#include <iostream>
#include <string>

#include <a/prelude.hpp>


int main() {
    Lexer lexer;
    Parser parser;
    Transformer transformer;
    Checker checker;
    Emitter emitter;

    std::clog << "👉 reading..." << std::endl;
    auto tokens = lexer.tokenize(&std::cin);

    std::clog << "👉 parsing..." << std::endl;
    auto ast = parser.parse(std::move(tokens));
    ast->inspect(&std::clog);
    std::clog << std::endl;

    std::clog << "👉 verifying..."<< std::endl;
    checker.check(ast);

    std::clog << "👉 converting..." << std::endl;
    transformer.transform(ast);
    ast->inspect(&std::clog);
    std::clog << std::endl;

    std::clog << "👉 emitting..." << std::endl;
    emitter.feed(ast);

    emitter.print();

    return 0;
}
