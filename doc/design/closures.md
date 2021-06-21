# Closures

The language is supposed to have a Scheme-like syntax while having
C-like internal implementation.

What this means is that there are essentially no S-expressions
and any runtime interpreter for them. Instead, everything is translated
into LLVM IR.

There can be added some runtime to add better support for functional-style
things like closures.

It's possible to validate variable scopes in compile time, so looks like
the only thing we need to find a solution for is how we should implement
functions that capture variables. Let's look at an example:

```
(define n 20)

(define (fun x)
    (+ x n)
)

(print (fun 30)) # -> prints 50
```

As all variables are immutable, there is no need to find a way to reference
them, especially that in a language without runtime/gc that would require some
trickery to avoid referencing a variable that's been popped from the stack.

And more than that, as functions can be created dynamically, we need to find
a way to __dynamically__ capture the variables. I think I have a solution,
but let's look at this example first:

```
(define (mk_fun num)
    (define (fun x)
        (+ x n)
    )
    fun
)

(define add_20 (mk_fun 20))
(define add_1 (mk_fun 20))

(print (add_20 30)) # -> prints 50
(print (add_20 50)) # -> prints 70

(print (add_1 1)) # -> prints 2
(print (add_1 3)) # -> prints 4
```

The thing here is that we capture the num variable from `mk_fun` and return
a lambda. As mentioned earlier, this lambda can't just reference `num` as
`num` is on stack and disappears after `mk_fun` returns. We can't also just
write `num`'s value into some static variable that we'd use in the `fun`,
because we should support creation of multiple `fun`s.

That's probably a pretty obvious solution, but I suggest adding a special type
of value that works as a wrapper for functions, storing their captured
variables. All functions will have such wrapper, for functions that don't
reference variables the wrapper will be empty.
This means that we can't __construct__ new functions in runtime, but we can
create as many functions _similar_ to existing ones using different captured
values.

Let's say the wrapper is something like this (pseudo-code):
```
struct Function {
    Implementation* function_pointer;
    vector<Type> argument_types;
    vector<Value> captured_values;
    Type return_type;
};
```

```
(define (mk_fun num)
    (define (fun x)
        (+ x n)
    )
    fun
)

(mk_fun 3) -> a

a -> Function {
    function_pointer = fun,
    argument_types = {int},
    captured_values = {value of num},
    return_type = int,
};
```

Another example:
```
(define (mk_funs val)
    (define (fun1)
        val
    )

    (define (fun2 p)
        (+ p val)
    )

    '(fun1 fun2)
)

(mk_funs 10) -> [a, b]

a -> Function {
    function_pointer = fun1,
    argument_types = {},
    captured_values = {10},
    return_type = int,
};

b -> Function {
    function_pointer = fun2,
    argument_types = {int},
    captured_values = {10},
    return_type = int,
};
```
