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
    if(stack.empty()) {
        return NilAstNode::make();
    }

    std::vector<UpExprAstNode> nodes;
    nodes.reserve(stack.size());
    for(size_t i = 0; i < stack.size(); i++) {
        UpExprAstNode ast_node;
        if(std::holds_alternative<UpToken>(stack[i])) {
            UpToken tok = std::move(std::get<UpToken>(stack[i]));
            switch(tok->kind) {
            case TokenKind::special_form:
                ast_node = SpecialFormAstNode::make(dynamic_cast<TokenSpecialForm*>(tok.get())->value());
                break;
            case TokenKind::identifier:
                ast_node = IdentifierAstNode::make(dynamic_cast<TokenIdentifier*>(tok.get())->value());
                break;
            case TokenKind::integer:
                ast_node = IntegerAstNode::make(dynamic_cast<TokenInteger*>(tok.get())->value());
                break;
            default:
                throw CompilerError();
            }
        } else {
            ast_node = std::move(std::get<UpExprAstNode>(stack[i]));
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
