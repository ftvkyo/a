#include "a/prelude.hpp"


Token2Ast::Token2Ast() = default;


Token2Ast& Token2Ast::operator<<(pToken token) {
    switch(token->get_kind()) {
        case TokenKind::bracket_left:
        case TokenKind::bracket_right:
        case TokenKind::eof:
            throw CompilerError();
        case TokenKind::special_form:
        case TokenKind::identifier:
        case TokenKind::integer:
            stack.emplace_back(token);
            break;
    }

    return *this;
}


Token2Ast& Token2Ast::operator<<(pAst ast_node) {
    stack.emplace_back(ast_node);
    return *this;
}


pAst Token2Ast::extract() {
    std::vector<pAst> nodes;
    nodes.reserve(stack.size());
    for(auto token : stack) {
        pAst ast_node;
        if(std::holds_alternative<pToken>(token)) {
            pToken tok = std::get<pToken>(token);
            switch(tok->get_kind()) {
            case TokenKind::special_form:
                ast_node = AstSpecialForm::make(tok->retrieve_symbol());
                break;
            case TokenKind::identifier:
                ast_node = AstIdentifier::make(tok->retrieve_symbol());
                break;
            case TokenKind::integer:
                ast_node = AstInteger::make(tok->retrieve_int());
                break;
            default:
                throw CompilerError();
            }
        } else {
            ast_node = std::get<pAst>(token);
        }
        nodes.emplace_back(ast_node);
    }
    stack.clear();

    return AstSequence::make(std::move(nodes));
}


void Token2Ast::operator>>(Token2Ast& receiver) {
    receiver << this->extract();
}
