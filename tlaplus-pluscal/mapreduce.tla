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
CONSTANTS Workers, Books
ASSUME Books /= {<<>>}

(* --algorithm mapreduce
variables
    books \in Books,
    jobQueue = [w \in Workers |-> <<>>],
    mapResult = [w \in Workers |-> 0],

process reducer = 1
variables
    assignedWorkers = [w \in Workers |-> FALSE],
    totalWordCount = 0,
begin
    AssignJob:
    while books /= <<>> do
        WorkerBatch:
        while \E w \in Workers: ~assignedWorkers[w] do
            with w \in {w \in Workers: ~assignedWorkers[w]} do
                assignedWorkers[w] := TRUE;
                jobQueue[w] := Append(jobQueue[w], Head(books));
                books := Tail(books);
                if books = <<>> then
                    goto Reduce;
                end if;
            end with;
        end while;
    end while;

    Reduce:
    while \E w \in Workers: assignedWorkers[w] do
        with w \in {w \in Workers: assignedWorkers[w]} do
            if mapResult[w] /= 0 then
                totalWordCount := totalWordCount + mapResult[w];
                assignedWorkers[w] := FALSE;
            end if 
        end with;
    end while;

    Finish:
    assert totalWordCount = 4;
end process;

process worker \in Workers
variables
    total = 0,
begin
    WaitForJobs:
    await jobQueue[self] /= <<>>;

    Map:
    while jobQueue[self] /= <<>> do
        total := total + Head(jobQueue[self]);
        jobQueue[self] := Tail(jobQueue[self]);
    end while;

    MapDone:
    mapResult[self] := total;
end process;

end algorithm; *)
\* BEGIN TRANSLATION
VARIABLES books, jobQueue, mapResult, pc, assignedWorkers, totalWordCount, 
          total

vars == << books, jobQueue, mapResult, pc, assignedWorkers, totalWordCount, 
           total >>

ProcSet == {1} \cup (Workers)

Init == (* Global variables *)
        /\ books \in Books
        /\ jobQueue = [w \in Workers |-> <<>>]
        /\ mapResult = [w \in Workers |-> 0]
        (* Process reducer *)
        /\ assignedWorkers = [w \in Workers |-> FALSE]
        /\ totalWordCount = 0
        (* Process worker *)
        /\ total = [self \in Workers |-> 0]
        /\ pc = [self \in ProcSet |-> CASE self = 1 -> "AssignJob"
                                        [] self \in Workers -> "WaitForJobs"]

AssignJob == /\ pc[1] = "AssignJob"
             /\ IF books /= <<>>
                   THEN /\ pc' = [pc EXCEPT ![1] = "WorkerBatch"]
                   ELSE /\ pc' = [pc EXCEPT ![1] = "Reduce"]
             /\ UNCHANGED << books, jobQueue, mapResult, assignedWorkers, 
                             totalWordCount, total >>

WorkerBatch == /\ pc[1] = "WorkerBatch"
               /\ IF \E w \in Workers: ~assignedWorkers[w]
                     THEN /\ \E w \in {w \in Workers: ~assignedWorkers[w]}:
                               /\ assignedWorkers' = [assignedWorkers EXCEPT ![w] = TRUE]
                               /\ jobQueue' = [jobQueue EXCEPT ![w] = Append(jobQueue[w], Head(books))]
                               /\ books' = Tail(books)
                               /\ IF books' = <<>>
                                     THEN /\ pc' = [pc EXCEPT ![1] = "Reduce"]
                                     ELSE /\ pc' = [pc EXCEPT ![1] = "WorkerBatch"]
                     ELSE /\ pc' = [pc EXCEPT ![1] = "AssignJob"]
                          /\ UNCHANGED << books, jobQueue, assignedWorkers >>
               /\ UNCHANGED << mapResult, totalWordCount, total >>

Reduce == /\ pc[1] = "Reduce"
          /\ IF \E w \in Workers: assignedWorkers[w]
                THEN /\ \E w \in {w \in Workers: assignedWorkers[w]}:
                          IF mapResult[w] /= 0
                             THEN /\ totalWordCount' = totalWordCount + mapResult[w]
                                  /\ assignedWorkers' = [assignedWorkers EXCEPT ![w] = FALSE]
                             ELSE /\ TRUE
                                  /\ UNCHANGED << assignedWorkers, 
                                                  totalWordCount >>
                     /\ pc' = [pc EXCEPT ![1] = "Reduce"]
                ELSE /\ pc' = [pc EXCEPT ![1] = "Finish"]
                     /\ UNCHANGED << assignedWorkers, totalWordCount >>
          /\ UNCHANGED << books, jobQueue, mapResult, total >>

Finish == /\ pc[1] = "Finish"
          /\ Assert(totalWordCount = 4, 
                    "Failure of assertion at line 55, column 5.")
          /\ pc' = [pc EXCEPT ![1] = "Done"]
          /\ UNCHANGED << books, jobQueue, mapResult, assignedWorkers, 
                          totalWordCount, total >>

reducer == AssignJob \/ WorkerBatch \/ Reduce \/ Finish

WaitForJobs(self) == /\ pc[self] = "WaitForJobs"
                     /\ jobQueue[self] /= <<>>
                     /\ pc' = [pc EXCEPT ![self] = "Map"]
                     /\ UNCHANGED << books, jobQueue, mapResult, 
                                     assignedWorkers, totalWordCount, total >>

Map(self) == /\ pc[self] = "Map"
             /\ IF jobQueue[self] /= <<>>
                   THEN /\ total' = [total EXCEPT ![self] = total[self] + Head(jobQueue[self])]
                        /\ jobQueue' = [jobQueue EXCEPT ![self] = Tail(jobQueue[self])]
                        /\ pc' = [pc EXCEPT ![self] = "Map"]
                   ELSE /\ pc' = [pc EXCEPT ![self] = "MapDone"]
                        /\ UNCHANGED << jobQueue, total >>
             /\ UNCHANGED << books, mapResult, assignedWorkers, totalWordCount >>

MapDone(self) == /\ pc[self] = "MapDone"
                 /\ mapResult' = [mapResult EXCEPT ![self] = total[self]]
                 /\ pc' = [pc EXCEPT ![self] = "Done"]
                 /\ UNCHANGED << books, jobQueue, assignedWorkers, 
                                 totalWordCount, total >>

worker(self) == WaitForJobs(self) \/ Map(self) \/ MapDone(self)

(* Allow infinite stuttering to prevent deadlock on termination. *)
Terminating == /\ \A self \in ProcSet: pc[self] = "Done"
               /\ UNCHANGED vars

Next == reducer
           \/ (\E self \in Workers: worker(self))
           \/ Terminating

Spec == Init /\ [][Next]_vars

Termination == <>(\A self \in ProcSet: pc[self] = "Done")

\* END TRANSLATION

Invariant == TRUE

=============================================================================
\* Modification History
\* Last modified Sat Jan 04 23:04:50 ICT 2020 by so61pi
\* Created Sat Jan 04 19:13:39 ICT 2020 by so61pi
