#include "a.hpp"


TokenToAst::TokenToAst() = default;


TokenToAst& TokenToAst::operator<<(pToken&& token) {
    switch(token->kind) {
        case TokenKind::bracket_left:
        case TokenKind::bracket_right:
        case TokenKind::eof:
            throw CompilerError();
        case TokenKind::special_form:
        case TokenKind::identifier:
        case TokenKind::integer:
            stack.emplace_back(std::move(token));
            break;
    }

    return *this;
}


TokenToAst& TokenToAst::operator<<(pAst&& ast_node) {
    stack.emplace_back(std::move(ast_node));
    return *this;
}


pAst TokenToAst::extract() {
    std::vector<pAst> nodes;
    nodes.reserve(stack.size());
    for(auto&& token : stack) {
        pAst ast_node;
        if(std::holds_alternative<pToken>(token)) {
            pToken tok = std::move(std::get<pToken>(token));
            switch(tok->kind) {
            case TokenKind::special_form:
                ast_node = AstSpecialForm::make(tok->get_string());
                break;
            case TokenKind::identifier:
                ast_node = AstIdentifier::make(tok->get_string());
                break;
            case TokenKind::integer:
                ast_node = AstInteger::make(tok->get_int());
                break;
            default:
                throw CompilerError();
            }
        } else {
            ast_node = std::move(std::get<pAst>(token));
        }
        nodes.emplace_back(std::move(ast_node));
    }
    stack.clear();

    return AstSequence::make(std::move(nodes));
}


void TokenToAst::operator>>(TokenToAst& receiver) {
    receiver << this->extract();
}


Parser::Parser() = default;

pAst Parser::parse(std::vector<pToken>&& tokens) {
    std::vector<TokenToAst> converters(20);
    size_t depth = 0;

    converters[0] << AstSpecialForm::make("@block");

    for(auto& tok : tokens) {
        switch(tok->kind) {
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
