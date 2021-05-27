doctest_inc = include_directories('../deps/doctest/doctest')

testexe = executable(
    'testexe',
    'main.cpp',
    include_directories: [a_inc, doctest_inc],
    link_with: a_lib
)

test('All tests', testexe)
