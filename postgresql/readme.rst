.. contents::


Softwares
=========

- postgresql
    - Tag: ``REL_11_2``
    - Hash: ``6cd404b344f7e27f4d64555bb133f18a758fe851``


Notes
=====

- Left table is outer table, right table is inner table.

    .. code-block:: sql

        SELECT * FROM tbl_outer AS outer, tbl_inner AS inner WHERE inner.attr1 = outer.attr2;


Query Execution Flow
====================

- query string -> ``RawStmt`` -> ``Query`` -> ``PlannedStmt{Plan}`` -> ``Plan`` + ``PlanState`` -> ``ExprEvalStep{ ExprEvalOp }`` array -> retrive tuple/row until NULL, check it against expression array

.. code-block::

    PostgresMain
    `- exec_simple_query
       |- pg_parse_query(query string) -> RawStmt tree
       |  `- raw_parser
       |- pg_analyze_and_rewrite
       |  |- parse_analyze(RawStmt tree) -> Query tree
       |  |  `- transformTopLevelStmt
       |  |     `- transformOptionalSelectInto
       |  |        `- transformStmt
       |  `- pg_rewrite_query
       |- pg_plan_queries
       |  `- pg_plan_query(Query tree) -> PlannedStmt{ Plan tree } tree
       |     `- planner
       |        `- standard_planner
       |           |- subquery_planner                                                                              // Create plans
       |           |- ...                                                                                           // Select best path
       |           `- create_plan(best_path)
       |              `- create_plan_recurse
       |                 `- case T_SeqScan: create_scan_plan(best_path) -> Plan
       |                    `- case T_SeqScan: create_seqscan_plan -> SeqScan{ Plan }
       `- PortalRun
          `- PortalRunMulti
             `- ProcessQuery
                |- CreateQueryDesc(PlannedStmt tree, Snapshot snapshot) -> QueryDesc{ PlannedStmt tree }
                |- ExecutorStart(QueryDesc)
                |  `- standard_ExecutorStart
                |     `- InitPlan
                |        |- ExecCheckRTPerms                                                                        // Do permissions checks
                |        |- ...                                                                                     // Lock relations selected FOR [KEY] UPDATE/SHARE
                |        `- ExecInitNode(Plan tree) -> PlanState tree                                               // Constructs PlanState tree from Plan tree
                |           `- case T_SeqScan: ExecInitSeqScan(SeqScan node) -> SeqScanState node
                |              |- set callback ExecProcNode = ExecSeqScan
                |              `- ExecInitQual
                |                 |- ExecInitExprRec                                                                // Convert Expr to steps...
                |                 |  `- ExprEvalPushStep(ExprState node, ExprEvalStep{ ExprEvalOp } step)           // .. then add step to node
                |                 `- ExecReadyExpr
                |                    `- ExecReadyInterpretedExpr (or jit_compile_expr)
                |                       |- set state->evalfunc = ExecInterpExprStillValid
                |                       `- set state->evalfunc_private = ExecInterpExpr
                |- ExecutorRun(QueryDesc)
                |  `- standard_ExecutorRun
                |     `- ExecutePlan(PlanState tree)
                |        `- ExecProcNode(PlanState node)
                |           `- node->ExecProcNode(node) = ExecSeqScan(PlanState node)                               // Scans the values lists sequentially and returns the next qualifying tuple (row)
                |              `- ExecScan(ScanState node) 
                |                 |- ExecScanFetch(ScanState node)
                |                 |  `- accessMtd = SeqNext(SeqScanState node)                                      // ** Get next tuple/row **
                |                 `- ExecQual                                                                       // ** Check if row is qualified **
                |                    `- ExecEvalExprSwitchContext
                |                       `- state->evalfunc(state, econtext, isNull) = ExecInterpExprStillValid(ExprState *state, ExprContext *econtext, bool *isNull) -> Datum
                |                          |- state->evalfunc = (ExprStateEvalFunc) state->evalfunc_private;
                |                          `- return state->evalfunc(state, econtext, isNull); = ExecInterpExpr     // Interpret ExprEvalOp array
                |- ExecutorFinish(QueryDesc)
                `- ExecutorEnd(QueryDesc)


References
----------

- https://www.postgresql.org/developer/backend/
- postgresql/src/backend/parser/README
- postgresql/src/backend/optimizer/README
- postgresql/src/backend/executor/README
- http://patshaughnessy.net/2014/10/13/following-a-select-statement-through-postgres-internals


Query Plan
==========

Example
-------

.. code-block:: sql

    EXPLAIN ANALYZE
    SELECT *
    FROM bookings INNER JOIN members ON bookings.memid = members.memid;

.. code-block::

                                                        QUERY PLAN                                                     
    -------------------------------------------------------------------------------------------------------------------
     Hash Join  (cost=11.12..97.08 rows=4044 width=1474) (actual time=0.113..3.805 rows=4044 loops=1)
       Hash Cond: (bookings.memid = members.memid)
       ->  Seq Scan on bookings  (cost=0.00..74.44 rows=4044 width=36) (actual time=0.020..0.914 rows=4044 loops=1)
       ->  Hash  (cost=10.50..10.50 rows=50 width=1438) (actual time=0.034..0.034 rows=31 loops=1)
             Buckets: 1024  Batches: 1  Memory Usage: 12kB
             ->  Seq Scan on members  (cost=0.00..10.50 rows=50 width=1438) (actual time=0.012..0.018 rows=31 loops=1)
     Planning Time: 0.277 ms
     Execution Time: 4.221 ms

.. code-block::

    cost=start-up-cost..total-cost
    start-up-cost = cost expended before first tuple is retrieved
    total-cost = start-up-cost + run-cost
    run-cost = cost to fetch all tuples

Query plan is carried out inside out, as follows

#. Scan ``members`` table sequentially to retrieve rows
#. Construct a hash table with retrieved rows.
#. Scan ``bookings`` table sequentially.
#. Do a hash join on 2 tables above.


Notes
-----

- ``EXPLAIN`` only prints out query plan without executing the query.
- ``EXPLAIN ANALYZE`` actually executec the query.


References
----------

- postgresql/src/backend/optimizer/path/costsize.c
- https://www.postgresql.org/docs/11/using-explain.html


Logging/Debug Configuration Options
===================================

Configuration, Data And Log Location
------------------------------------

.. code-block:: sql

    show config_file;
    show data_directory;
    show log_directory;


Common Options
--------------

.. code-block::

    jit
    jit_debugging_support
    jit_dump_bitcode
    jit_expressions
    jit_profiling_support
    jit_tuple_deforming
    jit_above_cost
    jit_optimize_above_cost
    jit_inline_above_cost
    jit_provider

    logging_collector

    debug_assertions
    debug_print_parse
    debug_print_rewritten
    debug_print_plan
    debug_pretty_print
    debug_deadlocks

    log_checkpoints
    log_connections
    log_disconnections
    log_replication_commands
    log_duration
    log_parser_stats
    log_planner_stats
    log_executor_stats
    log_statement_stats
    log_btree_build_stats
    log_lock_waits
    log_hostname
    log_truncate_on_rotation
    log_file_mode
    log_min_duration_statement
    log_autovacuum_min_duration
    log_rotation_age
    log_rotation_size
    log_temp_files
    log_line_prefix
    log_timezone
    log_destination
    log_directory
    log_filename
    log_error_verbosity
    log_min_messages
    log_min_error_statement
    log_statement
    log_statement_stats
    log_statement_stats
    log_parser_stats
    log_planner_stats
    log_executor_stats

    trace_notify
    trace_locks
    trace_userlocks
    trace_lwlocks
    trace_sort
    trace_syncscan
    trace_lock_oidmin
    trace_lock_table
    trace_recovery_messages


Show Or Set Option In A Session
-------------------------------

.. code-block:: sql

    show <option>;

    set <option> to <value>;
    set <option> to default;

To make changes persistent, update options in ``config_file``, then restart postgres service.


Enable Logging Internal Backend Data Flow
-----------------------------------------

.. code-block:: sql

    set debug_print_parse to 'on';
    set debug_print_rewritten to 'on';
    set debug_print_plan to 'on';
    set debug_pretty_print to 'on';

- Log file is placed in ``data_directory/log/``


Enable LLVM JIT Compiler To Inspect Generated Expression
--------------------------------------------------------

.. code-block:: sql

    set jit to on;
    set jit_dump_bitcode to on;
    set jit_expressions to on;
    set jit_above_cost to 10;
    set jit_optimize_above_cost to -1;
    set jit_inline_above_cost to -1;
    set jit_tuple_deforming to off;

- Bitcode file is placed in ``data_directory``, to decompile bitcode or print CFG, use

    .. code-block:: shell

        llvm-dis -o file.ll file.bc
        opt -dot-cfg -o /dev/null file.bc


References
----------

- postgresql/src/backend/utils/misc/guc.c
- https://llvm.org/docs/LangRef.html


References
==========

- PostgreSQL offical documents
    - https://www.postgresql.org/docs/11/acronyms.html
    - https://www.postgresql.org/docs/11/index.html
    - `Internals <https://www.postgresql.org/docs/11/internals.html>`_
    - `Database File Layout <https://www.postgresql.org/docs/11/storage-file-layout.html>`_
    - `System Column <https://www.postgresql.org/docs/11/ddl-system-columns.html>`_
    - `Genetic Query Optimizer <https://www.postgresql.org/docs/11/geqo.html>`_
    - `Parallel Query <https://www.postgresql.org/docs/11/parallel-query.html>`_
    - `Performance Tips <https://www.postgresql.org/docs/11/performance-tips.html>`_
- External
    - http://www.interdb.jp/pg/
    - https://momjian.us/main/presentations/internals.html
    - https://brandur.org/postgres-atomicity
