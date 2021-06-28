a_sources = [
    's1-lexing/lexer.cpp',
    's1-lexing/token_matcher.cpp',
    's1-lexing/token.cpp',

    's2-parsing/ast.cpp',
    's2-parsing/parser.cpp',
    's2-parsing/token2ast.cpp',

    's3-verification/checker.cpp',

    's4-transformation/transformer.cpp',

    's5-codegen/context.cpp',
    's5-codegen/emitter.cpp',

    'error.cpp',
]

a_lib = shared_library(
    'a',
    a_sources,
    include_directories: a_inc,
    dependencies: llvm,
)
