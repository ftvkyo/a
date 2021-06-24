# Language description
An attempt to make an LLVM frontend for something.

This something is a C-like language with Scheme-like syntax.
Imagine you take C and replace its keywords with Scheme's
special forms and parentheses.

These implementation details are not very important as they probably
do not affect the user. The internals might be LISPified or whatever
in the future, the description above explains just the current state of things.

[Extended info](./doc/DOC.md)

# Compiling and testing

```console
 $ CC=clang CXX=clang++ meson builddir && cd builddir
 $ meson compile
 $ test/testexe
```

# Code examples

```
code examples go here
some day
```
