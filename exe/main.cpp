#include <iostream>
#include <string>

#include <a/prelude.hpp>


int main() {
    Lexer lexer;
    Parser parser;
    Converter converter;
    Checker checker;
    Emitter emitter;

    std::cout << "reading..." << std::endl;
    auto tokens = lexer.tokenize(&std::cin);

    std::cout << "parsing..." << std::endl;
    auto ast = parser.parse(std::move(tokens));

    std::cout << "verifying..."<< std::endl;
    checker.check(ast);

    std::cout << "converting..." << std::endl;
    converter.convert(ast);

    std::cout << "emitting..." << std::endl;
    emitter.feed(ast);

    emitter.print();

    return 0;
}
