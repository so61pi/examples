.. contents:: Table of Contents

General Checks For Function ``F``
=================================

#. Does ``F`` have only one single purpose?
#. Does ``F``'s set of valid values equal to the product of all sets of valid values of ``F``'s inputs?

   - How are the undesirable combinations kept out?
   - Can types that are more specialized/fundamental be used for inputs instead?

#. Do ``F``'s inputs have any special combination?
#. Does ``F`` invoke global components (e.g. time, filesystem, database...)?

   - Can those invocations be moved outside to make ``F`` **functional**?

#. Is ``F`` written in favor of readability instead of performance?
#. Are ``F``'s inputs placed in **(in, in-out)** order?
#. Are ``F``'s outputs placed in **(out, error)** order?
#. Variables

   - Does each variable have only one single purpose?
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
