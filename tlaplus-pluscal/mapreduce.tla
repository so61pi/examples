----------------------------- MODULE mapreduce -----------------------------
\* *** Requirements from Practical TLA+ chapter 11. ***
\*
\* 1. The first computer is labeled the reducer. The other three are labeled workers.
\* 2. The reducer assigns books 1, 4, 7 ... to the first worker, 2, 5, 8... to the second worker, and the remaining books to the third worker.
\* 3. Each worker calculates the number of words in its assigned books and reports that back to the reducer.
\* 4. The reducer sums the numbers together to get the final wordcount.
\*
\* *** Implementation stages ***
\*
\* 1. A first spec that assumes all workers always succeed.
\* 2. A second, fault tolerant spec that allows workers to fail.
\* 3. A final spec that works even if the recovery mechanism partially fails, too.
EXTENDS Integers, TLC, Sequences
CONSTANTS Workers, Books, NumWordCounts

(* --algorithm mapreduce
variables
    dummy = {},

define
    Dummy == dummy
end define;

process reduce = 1
variables
begin
end process;

process worker \in Workers
variables
begin
end process;

end algorithm; *)
\* BEGIN TRANSLATION
VARIABLE dummy

(* define statement *)
Dummy == dummy


vars == << dummy >>

ProcSet == {1} \cup (Workers)

Init == (* Global variables *)
        /\ dummy = {}

Next == reduce
           \/ (\E self \in Workers: worker(self))

Spec == Init /\ [][Next]_vars

\* END TRANSLATION

Invariant == TRUE

=============================================================================
\* Modification History
\* Last modified Sat Jan 04 19:45:11 ICT 2020 by so61pi
\* Created Sat Jan 04 19:13:39 ICT 2020 by so61pi
