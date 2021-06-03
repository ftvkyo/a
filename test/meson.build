doctest_inc = include_directories('../deps/doctest/doctest')

test_sources = [
    'main.cpp',
    'token.cpp',
    'lexer.cpp',
    'parser.cpp',
]

testexe = executable(
    'testexe',
    test_sources,
    include_directories: [a_inc, doctest_inc],
    link_with: a_lib,
)

test('All tests', testexe)
