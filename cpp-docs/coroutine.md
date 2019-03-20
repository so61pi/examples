## Document

- http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2018/n4736.pdf

## co_await

```c++
/*  Syntax
 *
 *  await-expression:
 *      co_await cast-expression
 */

auto&& e = cast-expression

if (!e.await_ready()) {
    // coroutine is considered suspended
    e.await_suspend(h);

    // ... suspend/resume point ...
}

auto result = e.await_resume();
```

The `await-expression` evaluates the `await-ready` expression, then:

- If the result is `false`, the coroutine is considered suspended. Then, the `await-suspend` expression is evaluated. If that expression has type `std::experimental::coroutine_handle<Z>` and evaluates to a value `s`, the coroutine referred to by `s` is resumed as if by a call `s.resume()`. *[ Note: **Any number of coroutines may be successively resumed in this fashion, eventually returning control flow to the current coroutine caller or resumer** (11.4.4). — end note ]* If that expression has type `bool` and evaluates to `false`, the coroutine is resumed. If that expression exits via an exception, the exception is caught, the coroutine is resumed, and the exception is immediately re-thrown (18.1). Otherwise, control flow returns to the current coroutine caller or resumer (11.4.4) without exiting any scopes (9.6).
- If the result is `true`, or when the coroutine is resumed, the `await-resume` expression is evaluated, and its result is the result of the `await-expression`.

## co_yield

```c++
/*  Syntax
 *
 *  yield-expression:
 *      co_yield assignment-expression
 *      co_yield braced-init-list
 */

co_await p.yield_value(expression);
```

## co_return

```c++
/*  Syntax
 *
 *  coroutine-return-statement:
 *      co_return expr-or-braced-init-list
 */

// to set the value of expression into p so the outside function can get it
p.return_value(expression);

goto final_suspend;
```

## for co_wait

```c++
/*  Syntax
 *
 *  for co_await (for-range-declaration : for-range-initializer)
 *      statement
 */

{
    auto &&__range = for-range-initializer;
    auto __begin = co_await begin-expr;
    auto __end = end-expr;
    for ( ; __begin != __end; co_await ++__begin ) {
        for-range-declaration= *__begin;
        statement
    }
}
```

## High level view of generated code of coroutine

```c++
/*  f is a coroutine, F is the function body of f
 */

{
    P p;

    // initial suspend point
    co_await p.initial_suspend();

    try {
        F
    } catch(...) {
        p.unhandled_exception();
    }

final_suspend:
    co_await p.final_suspend(); // final suspend point
}
```

## coroutine_handle resumption member function

```txt
void operator()();
void resume();

Requires: *this refers to a suspended coroutine.
Effects: resumes the execution of the coroutine. If the coroutine was suspended at its final suspend point, behavior is undefined.
Synchronization: a concurrent resumption of the coroutine via resume, operator(), or destroy may result in a data race.
```

```txt
void destroy();

Requires: *this refers to a suspended coroutine.
Effects: destroys the coroutine (11.4.4).
Synchronization: a concurrent resumption of the coroutine via resume, operator(), or destroy may result in a data race.
```

- A suspended coroutine can be resumed to continue execution by invoking a resumption member function (21.11.2.4) of an object of type `coroutine_handle<P>` associated with this instance of the coroutine. **The function that invoked a resumption member function is called resumer.**
- Invoking a resumption member function for a coroutine that is not suspended results in undefined behavior.
- A common scenario is to launch async activity from within `await_suspend` and pass a `coroutine_handle`, that was helpfully provided to `await_suspend`, as a context to whatever async activity you are launching. When it completes, you are free to resume the coroutine. No extra synchronization is needed. (https://www.reddit.com/r/cpp/comments/6wy3sv/is_coroutine_considered_suspended_during_await/)

```c++
// https://github.com/GorNishanov/await/blob/master/2017_CppCon/live/hard1.cpp
void await_suspend(coroutine_handle<> h) {
    s.async_read_some(b, [this, h](auto ec, auto n) mutable {
        this->ec = ec;
        this->n = n;
        h.resume();
    });
}

// lambda above is the resumer (it calls coroutine_handle::resume)
```
