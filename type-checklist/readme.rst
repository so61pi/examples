.. contents:: Table of Contents

Concepts
========

Basic
-----

    - An **abstract entity** is an individual thing that is eternal and unchangeable (e.g. blue, 13).

-----

    - A value type is **properly partial** if its values represent a proper *subset of the abstract entities* in the corresponding species; otherwise it is **total**.
    - A value type is **uniquely represented** if and only if *at most one value corresponds to each abstract entity*.
    - A value type is **ambiguous** if and only if *a value of the type has more than one interpretation*. The negation of ambiguous is **unambiguous**.

-----

    - Two values are **equal** if and only if they represent a same abstract entity.
    - They are **representationally equal** if and only if their datums are identical sequences of 0s and 1s.

      * If a value type is **uniquely represented**, *equality implies representational equality*.
      * If a value type is **unambiguous**, *representational equality implies equality*.

Computational Basis
-------------------

    A **computational basis** for a type is a finite set of procedures that enable the construction of any other procedure on the type.

    A basis is **efficient** if and only if any procedure implemeted using it is as efficient as an equivalent procedure written in terms of an alternative basis.

    A basis is **expressive** if and only if it allows compact and convenient definitions of procedures on the type.

Regular Types
-------------

    A type is **regular** if and only if its basis includes

    - equality
    - assignment
    - destructor
    - default constructor
    - copy constructor
    - total ordering

Whole-Part Relationship
-----------------------

    A **whole-part relationship** is connected, non-circular, logically disjoint, and owned.

    **Connected** means any part is reachable from the object's starting address.

    **Non-circular** means that no part is a part of itself.

    **Logically disjoint** means that if two objects share a subpart where modifications to
    the subpart affect the value of both objects, then one of the objects must be a subpart
    of the other.

    **Owned** means that copying the object copies its parts, and destroying the object destroys
    its parts.

**Non-circular** actually means if A has an owning pointer to B then B cannot have an owning pointer to A.
(B still can have non-owning pointer to A though.)

**Logically disjoint** actually means if A and B both point to C and we are able to change C then A must contain B or vice versa.
In other words, we cannot have 2 independent objects sharing a same modifiable object. (Note that if the shared object
is immutable, then the relationship is always logically disjoint.)

Relations
---------

``r(a: T, b: T) -> bool``

Transitive
~~~~~~~~~~

::

    r(a, b) && r(b, c) => r(a, c)

Example:

- equal
- less
- greater

Strict
~~~~~~

::

    !r(a, a)

Example:

- less
- greater

Reflexive
~~~~~~~~~

::

    r(a, a)

Example:

- equal

Symmetric
~~~~~~~~~

::

    r(a, b) => r(b, a)

Example:

- equal

Asymmetric
~~~~~~~~~~

::

    r(a, b) => !r(b, a)

Example:

- less

Equivalence
~~~~~~~~~~~

::

    Transitive + Reflexive + Symmetric

Example:

- equal

Total Ordering
~~~~~~~~~~~~~~

::

    Transitive && only one of {r(a, b); r(b, a); a == b}

Example:

- less
- greater

Weak Ordering
~~~~~~~~~~~~~

::

    Transitive && only one of {r(a, b); r(b, a); !r(a, b) && !r(b, a)}

Properties:

- Total ordering is weak ordering
- Weak ordering is asymmetric
- Weak ordering is strict

General Checks For Type ``T``
=============================

#. Does ``T`` have only one single purpose?

#. What is ``T``'s invariant?

#. Does ``T``'s **set of valid values** equal to **the product of all sets of valid values of T's components**?

   - How are the undesirable combinations kept out?
   - Can types that are more specialized/fundamental be used for ``T``'s components instead?

#. Does ``T`` have any special value (e.g. when all of its components are zero or null)?

   - How are they handled?

#. Does ``T`` have all of the below properties?

   - Regular
   - Whole-part relationship

#. What is the precondition of each method?

#. Is there any special order for method invocation (e.g. ``method_a`` must be called before ``method_b``)?

#. Does ``T``'s computational basis have all of the below properties?

   - Efficient
   - Expressive
   - Complete (able to construct and operate on any representable value)

References
==========

- Elements of Programming by Alexander A. Stepanov and Paul McJones 
- `Goal: Implement Complete & Efficient Types <https://sean-parent.stlab.cc/papers-and-presentations/#goal-implement-complete--efficient-types>`__
- `Better Code: Data Structures <https://sean-parent.stlab.cc/papers-and-presentations/#better-code-data-structures>`__
- `Better Code <https://sean-parent.stlab.cc/papers-and-presentations/#better-code>`__
