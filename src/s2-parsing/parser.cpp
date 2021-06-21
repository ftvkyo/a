#include "a/prelude.hpp"


Parser::Parser() = default;

pAst Parser::parse(std::vector<pToken>&& tokens) {
    std::vector<Token2Ast> converters(20);
    size_t depth = 0;

    converters[0] << AstSpecialForm::make("@block");

    for(auto& tok : tokens) {
        switch(tok->get_kind()) {
        case TokenKind::bracket_left:
            depth++;
            if(depth >= converters.size()) {
                converters.emplace_back();
            }
            break;
        case TokenKind::bracket_right:
            if(depth == 0) {
                throw SyntaxError();
            }
            depth--;
            converters[depth + 1] >> converters[depth];
            break;
        case TokenKind::special_form:
        case TokenKind::identifier:
        case TokenKind::integer:
            converters[depth] << std::move(tok);
        case TokenKind::eof:
            // Not an attempt to leave the cycle
            break;
        }
    }

    if(depth != 0) {
        throw SyntaxError();
    }

    return converters[0].extract();
}
