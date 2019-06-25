.. contents:: Table of Contents

General Checks For Function ``F``
=================================

Overview
--------

#. Does ``F`` have only one single purpose?

Inputs/Outputs
--------------

#. Does ``F``'s set of valid values equal to the product of all sets of valid values of ``F``'s inputs?

   - How are undesirable combinations avoided?
   - Can types that are more abstract be used for inputs instead?

#. Do ``F``'s inputs have any special combination?
#. Are ``F``'s inputs placed in **(in, in-out)** order?
#. Do ``F``'s inputs follow below convention?

   - In inputs that ``F`` consumes are passed by value.
   - In inputs that ``F`` does not consume are passed by const reference.
   - In-out inputs are passed by non-const reference.
   - Optional inputs should use ``Optional`` class (or something similar) instead of pointer.

     * Except for string or vector as their empty state can be treated as optional, but it should be documented.

#. Are ``F``'s outputs placed in **(out, error)** order?

Code
----

#. Is ``F`` written in favor of readability instead of performance?
#. Does ``F`` invoke global components (e.g. time, filesystem, database...)?

   - Can those invocations be moved outside to make ``F`` **functional**?

#. Variables

   - Does each variable have only one single purpose?
   - Can any variable have smaller scope?

     * How many lines are there between a variable's declaration and its last usage?

   - Is there any mutable variable that can be replaced with an immutable one?

#. Loops

   - Is there any loop that can be replaced with a well-known algorithm?

     * folding
     * sorting
     * searching
     * selecting

       + https://en.cppreference.com/w/cpp/algorithm/nth_element

     * partitioning
     * rotating

   - Does every loop have only one single purpose?
   - What is the invariant of each loop?
   - Can any loop be moved to its own function?

#. Ifs

   - Is there any ``if`` that handles special case?

     * Can that special case be turned into normal case?

   - Can any ``if`` have smaller scope?
   - Is there any complex ``if`` statement can be refactored to improve readability?

#. Recursion

   - What is the terminating case for this recursive function?

Users POV
---------

#. How easy is it for users to use ``F`` incorrectly?
#. Can users know and understand all possible errors that can be returned from ``F`` by doing below actions?

   - Reading ``F``'s document.
   - Or reading ``F``'s source code (without reading any other code).

#. Does ``F`` use error messages (stating errors without providing any hint to fix) or help messages?
