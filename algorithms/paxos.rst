.. contents:: Table of Contents

Paxos
=====

- Paxos is the algorithm to solve consensus problems of multiple participants in unreliable network environment.
- Basic Paxos only allows participants to reach consensus once. (To do this multiple times, see Multi-Paxos.)
- There are 3 roles in this algorithm. They are Suggester, Voter and Learner. Usually, one machine plays all three roles.
- Below is the flow of Basic Paxos

+-------------------+--------------------+----------------------+------------------------+-----------------------------------------------------+----------------------------------------------------------------------+
| Step              | Message            | Sender               | Receiver               | Message Content                                     | Message Handling Rules                                               |
+===================+====================+======================+========================+=====================================================+======================================================================+
| Step 1: Gaining   | Permission Request | Suggester (Proposer) | All Voters (Acceptors) | + Suggestion ID                                     | .. code-block:: text                                                 |
| permission to     |                    |                      |                        |                                                     |                                                                      |
| make a suggestion |                    |                      |                        |                                                     |     if requested Suggestion ID >= previously granted Suggestion ID { |
|                   |                    |                      |                        |                                                     |       grant                                                          |
|                   |                    |                      |                        |                                                     |     } else {                                                         |
|                   |                    |                      |                        |                                                     |       ignore or send Nack                                            |
|                   |                    |                      |                        |                                                     |     }                                                                |
|                   +--------------------+----------------------+------------------------+-----------------------------------------------------+----------------------------------------------------------------------+
|                   | Permission Granted | Voter (Acceptor)     | Suggester (Proposer)   | + Suggestion ID of corresponding Permission Request |                                                                      |
|                   |                    |                      |                        | + Last accepted Suggestion ID (if any)              |                                                                      |
|                   |                    |                      |                        | + Last accepted Value (if any)                      |                                                                      |
+-------------------+--------------------+----------------------+------------------------+-----------------------------------------------------+----------------------------------------------------------------------+
| Step 2: Making a  | Suggestion         | Suggester (Proposer) | All Voters (Acceptors) | + Suggestion ID of corresponding Permission Request | .. code-block:: text                                                 |
| suggestion        |                    |                      |                        | + Value                                             |                                                                      |
|                   |                    |                      |                        |                                                     |     if requested Suggestion ID >= previously granted Suggestion ID { |
|                   |                    |                      |                        |                                                     |       accept                                                         |
|                   |                    |                      |                        |                                                     |     } else {                                                         |
|                   |                    |                      |                        |                                                     |       ignore or send Nack                                            |
|                   |                    |                      |                        |                                                     |     }                                                                |
|                   +--------------------+----------------------+------------------------+-----------------------------------------------------+----------------------------------------------------------------------+
|                   | Accepted           | Voter (Acceptor)     | All Arbiters (Leaners) | + Suggestion ID of corresponding Permission Request |                                                                      |
|                   |                    |                      |                        | + Value (optional)                                  |                                                                      |
+-------------------+--------------------+----------------------+------------------------+-----------------------------------------------------+----------------------------------------------------------------------+

References
==========

- https://understandingpaxos.wordpress.com/
- https://en.wikipedia.org/wiki/Paxos_(computer_science)
- https://simbo1905.blog/2014/10/28/transaction-log-replication-with-paxos/
- https://news.ycombinator.com/item?id=11188917
