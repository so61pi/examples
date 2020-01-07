.. contents:: Table of Contents

Syntax Overview
===============

TLA+ Expressions
----------------

Numbers
~~~~~~~

``+ - * ^ < > <= >= % /``

Boolean Operators
~~~~~~~~~~~~~~~~~

======= =================
Symbol  Meaning
======= =================
``/\``  conjunction (and)
``\/``  disjunction (or)
``~``   negation (not)
``=>``  implication
``<=>`` equivalence
======= =================

======================================= =======
Expression                              Meaning
======================================= =======
``\A value \in set : Predicate(value)`` TRUE if every value of set makes ``Predicate`` TRUE
``\A value \in {} : Predicate(value)``  TRUE
``\E value \in set : Predicate(value)`` TRUE if at least one value of set makes ``Predicate`` TRUE
``\E value \in {} : Predicate(value)``  FALSE
======================================= =======

Sets
~~~~

============================================ =======
Expression                                   Meaning
============================================ =======
``value \in set``                            TRUE if ``set`` contains value
``set1 \union set2``                         Returns a set whose elements are from either ``set1`` or ``set2``
``set1 \intersect set2``                     Returns a set whose elements must appear in both ``set1`` and ``set2``
``set1 \subseteq set2``                      TRUE if every element of ``set1`` is an element of ``set2``, can be equal
``set1 \subset set2``                        TRUE if every element of ``set1`` is an element of ``set2``
``set1 \ set2``                              Returns a set whose elements are not in ``set2``
``SUBSET set``                               Returns a set of all subsets of ``set``, including empty set ``{}``. ``SUBSET {1, 2}`` is ``{{}, {1}, {2}, {1, 2}}``
``{value \in set : Predicate(value)}``       The subset of ``set`` whose every element satisfies ``Predicate``
``{Expr(value) : value \in set}``            Map ``set`` to new set
``value := CHOOSE i \in set : Predicate(i)`` Returns an arbitrary value of ``set`` that satisfies ``Predicate``. ``CHOOSE`` is nondeterministic, but it will always return the same value as the last chosen one.
============================================ =======

Functions
~~~~~~~~~

============================================ =======
Expression                                   Meaning
============================================ =======
``output := Function[input]``
``Function := [value \in set |-> formula]``
``Function := [i \in {1, 2, 3} |->  2 * i]``
``DOMAIN Function``                          Returns a set of all possible inputs of ``Function``
``[inputSet -> outputSet]``                  Returns a set of all functions ``Function`` with domain ``inputSet`` such that ``Function[input]`` is in ``outputSet`` for all ``input`` in ``inputSet``
``[{a, b} -> {1, 2}]``                       ``{ (a :> 1) @@ (b :> 1), (a :> 1) @@ (b :> 2), (a :> 2) @@ (b :> 1), (a :> 2) @@ (b :> 2) }``
============================================ =======

- ``:>`` and ``@@`` are from ``TLC`` module.

Records
~~~~~~~

- Records are functions whose doimain is the field names.
- ``[foo |-> 17, bar |-> {1, 2, 3}]`` is a record of the set ``[foo : Nat, bar : SUBSET 1..13]``

Tuples
~~~~~~

- Tuples are functions whose domains are indexes.
- Tuple elements can be addressed by index. Tuples are 1-indexed.
- ``<<value1, value2, value3>>[2]`` equals to ``value2``

======================== =======
Expression               Meaning
======================== =======
``Seq(set)``             Returns a set of all tuples of elements of ``set``
``Head(tuple)``          Returns first element of ``tuple``
``Tail(tuple)``          Returns a tuple without head
``Append(tuple, value)`` Returns a new tuple with appended value
``tuple1 \o tuple2``     Concat 2 tuples
``Len(tuple)``           Length of ``tuple``
======================== =======

Miscellaneous Constructs
~~~~~~~~~~~~~~~~~~~~~~~~

.. code-block:: text

    output := IF bool THEN t_expr ELSE e_expr

    output := CASE pred1 -> val1
                [] pred2 -> val2
                [] OTHER -> defval

    /* LET defines exprs, IN uses them
    output := LET x(a, b) == a + b
                  y       == 1 + 2
              IN  IF y > 0 THEN x(3, 4) + y ELSE x(5, 6) - y

Liveness
~~~~~~~~

A behavior is a sequence of states (``<<s1, s2, ...>>``).

================== =======
Expression         Meaning
================== =======
``[]Expr``         ``Expr`` is TRUE for every state of a behavior (always)
``<>Expr``         ``Expr`` is TRUE for at least one state of a behavior (eventually)
``Expr1 ~> Expr2`` ``Expr1`` becoming TRUE leads to ``Expr2`` becoming TRUE at current or some later state
``<>[]Expr``       ``Expr`` becomes TRUE at some state and stays TRUE afterwards up until the end of a behavior
``[]<>Expr``
================== =======

Definitions
~~~~~~~~~~~

- Operators::

	Function == expr
	Function(p1, ..., pn) == expr

- Recursive::

	f[n \in Nat] == IF n = 0 THEN 1 ELSE n * f[n - 1]

- Binary operators::

	a operator b == a + b

PlusCal (P-Syntax)
------------------

- Each label is an atomic action.
- Expressions in PlusCal can be any TLA+ expression.

Assignment::

    y := a + b
    a := b || b := a

``if``::

    if test then
        t_clause
    else
        e_clause
    end if;

``either`` arbitrarily chooses a clause to run::

    either
        clause1
    or
        clause2
    or
        clause3
    end either;

``while``::

    label:
    while test do
        body
    end while;

``await`` blocks waiting for ``expr`` to become TRUE::

    await expr;

``with`` run ``body`` by choosing an ``id`` from ``set`` nondeterministically::

    with id \in set do
        body
    end with;

``process``::

    [fair[+]] process ProcName \in IdSet
    [fair[+]] process ProcName = Id

``print`` and ``assert``::

    print expr;
    assert expr;

``define`` defines operators using global algorithm's variables using TLA+ expressions::

    variables x \in 1..10, y;
    define
        zy == y * (x + y)
        zx(a) == x * (y - a)
    end define;

PlusCal Template
================

TLA+ Standard Modules:

- Bags
- FiniteSets
- Integers
- Naturals
- RealTime
- Reals
- Sequences
- TLC

::

    ---- MODULE modname ----
    EXTENDS Integers, TLC, Sequences
    CONSTANTS Workers
    ASSUME Workers /= {}

    set ++ value == set \union {value}
    set -- value == set \ {value}
    Sum(seq) == 1

    (* --algorithm modname
    variables
        w \in Workers,
        x = 2;

    define
        X2 == x * 2
    end define;

    process procname \in Workers
    variables
        processLocalVariable = 0,
    begin
        skip;
    end process;

    end algorithm; *)
    \* BEGIN TRANSLATION
    \* END TRANSLATION

    Invariant == TRUE
    Liveness == <>[](TRUE)

    ====

References
==========

- `Practical TLA+ <https://www.apress.com/us/book/9781484238288>`__
- `Learn TLA+ <https://learntla.com>`__
- `A PlusCal User Manual P-Syntax Version 1.8 <https://lamport.azurewebsites.net/tla/p-manual.pdf>`__
