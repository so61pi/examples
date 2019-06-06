.. contents:: Table of Contents

Concepts
========

Basic
-----

+-------------------+---------------------------------------------------------------------------+-------------------+---------------------+
| Concept           | Definition                                                                | Example           | Represented by      |
+===================+===========================================================================+===================+=====================+
| Abstract entity   | An individual thing that is eternal and unchangeable.                     | - Blue            | Objects and values  |
|                   |                                                                           | - 13              |                     |
+-------------------+---------------------------------------------------------------------------+-------------------+---------------------+
| Abstract species  | Describes common properties of essentially equivalent abstract entities.  | - Color           | Types               |
|                   |                                                                           | - Natural number  |                     |
|                   |                                                                           | - Complex number  |                     |
+-------------------+---------------------------------------------------------------------------+-------------------+---------------------+
| Abstract genus    | Describes different abstract species that are similar in some respect.    | - Number          | Concepts/Traits     |
+-------------------+---------------------------------------------------------------------------+-------------------+---------------------+
| Datum             | A finite sequence of 0s and 1s                                            |                   |                     |
+-------------------+---------------------------------------------------------------------------+-------------------+---------------------+
| Type              | A correspondence between a species and a set of datums                    |                   |                     |
|                   |                                                                           |                   |                     |
|                   | - Datum is **representation** of entity                                   |                   |                     |
|                   | - Entity is **interpretation** of datum                                   |                   |                     |
|                   | - A datum is well formed if it represents an abstract entity              |                   |                     |
|                   |                                                                           |                   |                     |
|                   |   * NaN is not well formed                                                |                   |                     |
+-------------------+---------------------------------------------------------------------------+-------------------+---------------------+
| Value             | Datum + its interpretation                                                |                   |                     |
+-------------------+---------------------------------------------------------------------------+-------------------+---------------------+

Whole-Part Relationship
-----------------------

    A whole-part relationship is connected, non-circular, logically disjoint, and owned.

    Connected means any part is reachable from the object's starting address.

    Non-circular means that no part is a part of itself.

    Logically disjoint means that if two objects share a subpart where modifications to
    the subpart affect the value of both objects, then one of the objects must be a subpart
    of the other.

    Owned means that copying the object copies its parts, and destroying the object destroys
    its parts.

Logically disjoint means if A and B both point to C and we are able to change C then A must contain B or vice versa.
In other words, we cannot have 2 independent objects sharing a same modifiable object. (Note that if the shared object
is immutable, then the relationship of the pointee is always logically disjoint.)

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

General Checks For Types
========================

+---------------------------+---------------------------------------------------------------------------+---------------------------+
| Property / *inverse*      |                                                                           | Example / *inverse*       |
+===========================+===========================================================================+===========================+
| properly partial /        | **if** all of its values represent a proper subset of the abstract        | int / *bool*              |
| *total*                   | entities                                                                  |                           |
+---------------------------+---------------------------------------------------------------------------+---------------------------+
| uniquely represented      | **if and only if** at most one value corresponds to each abstract entity  | int / *bool represented   |
|                           |                                                                           | by a byte*                |
|                           | Note that one abstract entity still can be represented by multiple datums |                           |
+---------------------------+---------------------------------------------------------------------------+---------------------------+
| ambiguous / *unambiguous* | **if and only if** a value has more than one interpretation               | 99 is both 1999 and 2099  |
+---------------------------+---------------------------------------------------------------------------+---------------------------+
| regular                   | **if and only if** it has all these properties                            |                           |
|                           |                                                                           |                           |
|                           | - equality                                                                |                           |
|                           | - assignment                                                              |                           |
|                           | - destructor                                                              |                           |
|                           | - default constructor                                                     |                           |
|                           | - copy constructor                                                        |                           |
|                           | - total ordering                                                          |                           |
+---------------------------+---------------------------------------------------------------------------+---------------------------+
| complete                  | **if** the set of provided basis operations allows us to construct and    |                           |
|                           | operate on any representable value                                        |                           |
|                           |                                                                           |                           |
|                           |     A computational basis for a type is a finite set of procedures that   |                           |
|                           |     enable the construction of any other procedure on the type.           |                           |
+---------------------------+---------------------------------------------------------------------------+---------------------------+
| conform to whole-part     | **if and only if** it is                                                  |                           |
| relationship              |                                                                           |                           |
|                           | - connected                                                               |                           |
|                           | - non-circular                                                            |                           |
|                           | - logically disjoint                                                      |                           |
|                           | - owned                                                                   |                           |
+---------------------------+---------------------------------------------------------------------------+---------------------------+

- Two values are equal <=> they represent a same abstract entity (e.g. blue).
- They are representationally equal <=> their datums (e.g. ``00110001``) are identical.

  * A type is uniquely represented => equality implies representational equality
  * A type is unambiguous => representational equality implies equality

References
==========

- Elements of Programming by Alexander A. Stepanov, Paul McJones 
- `Goal: Implement Complete & Efficient Types <https://sean-parent.stlab.cc/papers-and-presentations/#goal-implement-complete--efficient-types>`__
- `Better Code: Data Structures <https://sean-parent.stlab.cc/papers-and-presentations/#better-code-data-structures>`__
- `Better Code <https://sean-parent.stlab.cc/papers-and-presentations/#better-code>`__
