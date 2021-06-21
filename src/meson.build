a_sources = [
    'a.cpp',

    'token.cpp',
    'lexer.cpp',

    'ast.cpp',
    'parser.cpp',

    'convert.cpp',

    'check.cpp',

    'emit.cpp',
]

a_lib = shared_library(
    'a',
    a_sources,
    include_directories: a_inc,
    dependencies: llvm,
)
