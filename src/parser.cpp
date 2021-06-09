#include "a.hpp"


TokenToAst::TokenToAst() = default;


TokenToAst& TokenToAst::operator<<(UpToken&& token) {
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


TokenToAst& TokenToAst::operator<<(UpExprAstNode&& ast_node) {
    stack.emplace_back(std::move(ast_node));
    return *this;
}


UpExprAstNode TokenToAst::extract() {
    std::vector<UpExprAstNode> nodes;
    nodes.reserve(stack.size());
    for(auto&& token : stack) {
        UpExprAstNode ast_node;
        if(std::holds_alternative<UpToken>(token)) {
            UpToken tok = std::move(std::get<UpToken>(token));
            switch(tok->kind) {
            case TokenKind::special_form:
                ast_node = SpecialFormAstNode::make(tok->get_string());
                break;
            case TokenKind::identifier:
                ast_node = IdentifierAstNode::make(tok->get_string());
                break;
            case TokenKind::integer:
                ast_node = IntegerAstNode::make(tok->get_int());
                break;
            default:
                throw CompilerError();
            }
        } else {
            ast_node = std::move(std::get<UpExprAstNode>(token));
        }
        nodes.emplace_back(std::move(ast_node));
    }
    stack.clear();

    return SeqAstNode::make(std::move(nodes));
}


void TokenToAst::operator>>(TokenToAst& receiver) {
    receiver << this->extract();
}


Parser::Parser() = default;

UpExprAstNode Parser::parse(std::vector<UpToken>&& tokens) {
    std::vector<TokenToAst> converters(20);
    size_t depth = 0;

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
