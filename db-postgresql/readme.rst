.. contents:: Table of Contents

Softwares
=========

- postgresql
    - Tag: ``REL_11_2``
    - Hash: ``6cd404b344f7e27f4d64555bb133f18a758fe851``

Internals
=========

Query Execution Flow
--------------------

Query string -> ``RawStmt`` -> ``Query`` -> ``PlannedStmt{Plan}`` -> ``Plan`` + ``PlanState`` -> ``ExprEvalStep{ ExprEvalOp }`` array -> retrieve tuple/row until NULL, check it against expression array

*Note:* The flow below might not be entirely correct.

.. code-block:: text

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
~~~~~~~~~~

- https://www.postgresql.org/developer/backend/
- postgresql/src/backend/parser/README
- postgresql/src/backend/optimizer/README
- postgresql/src/backend/executor/README
- http://patshaughnessy.net/2014/10/13/following-a-select-statement-through-postgres-internals

Query Plan
----------

Example
~~~~~~~

.. code-block:: sql

    EXPLAIN ANALYZE
    SELECT *
    FROM bookings INNER JOIN members ON bookings.memid = members.memid;

.. code-block:: text

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

.. code-block:: text

    cost=start-up-cost..total-cost
    start-up-cost = cost expended before first tuple is retrieved
    total-cost = start-up-cost + run-cost
    run-cost = cost to fetch all tuples

Query plan is carried out inside out, as follows

#. Scan ``members`` table sequentially to retrieve rows
#. Construct a hash table with retrieved rows.

   * Hash keys are set from outer ``Hash Join``, see postgresql/src/backend/executor/nodeHashjoin.c:744

#. Scan ``bookings`` table sequentially.
#. Do a hash join on 2 tables above.

Notes
~~~~~

- ``EXPLAIN`` only prints out query plan without executing the query.
- ``EXPLAIN ANALYZE`` actually executec the query.
- ``EXPLAIN VERBOSE`` displays more information.
- Left table is outer table, right table is inner table.

    .. code-block:: sql

        SELECT * FROM tbl_outer AS outer, tbl_inner AS inner WHERE inner.attr1 = outer.attr2;

References
~~~~~~~~~~

- https://www.postgresql.org/docs/11/using-explain.html
- https://www.postgresql.org/docs/11/sql-explain.html
- postgresql/src/backend/optimizer/path/costsize.c

Logging/Debug Configuration Options
-----------------------------------

Configuration, Data And Log Location
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. code-block:: sql

    SHOW config_file;
    SHOW data_directory;
    SHOW log_directory;

Common Options
~~~~~~~~~~~~~~

.. code-block:: text

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
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. code-block:: sql

    SHOW <option>;

    SET <option> TO <value>;
    SET <option> TO default;

To make changes persistent, update options in ``config_file``, then restart postgres service.

Enable Logging Internal Backend Data Flow
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. code-block:: sql

    SET debug_print_parse TO 'on';
    SET debug_print_rewritten TO 'on';
    SET debug_print_plan TO 'on';
    SET debug_pretty_print TO 'on';

- Log file is placed in ``data_directory/log/``

Enable LLVM JIT Compiler To Inspect Generated Expression
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. code-block:: sql

    SET jit TO on;
    SET jit_dump_bitcode TO on;
    SET jit_expressions TO on;
    SET jit_above_cost TO 10;
    SET jit_optimize_above_cost TO -1;
    SET jit_inline_above_cost TO -1;
    SET jit_tuple_deforming TO off;

- Bitcode file is placed in ``data_directory``, to decompile bitcode or print CFG, use

    .. code-block:: shell

        llvm-dis -o file.ll file.bc
        opt -dot-cfg -o /dev/null file.bc

References
~~~~~~~~~~

- https://llvm.org/docs/LangRef.html
- postgresql/src/backend/utils/misc/guc.c

References
----------

- PostgreSQL offical documents
    - https://www.postgresql.org/docs/11/acronyms.html
    - https://www.postgresql.org/docs/11/index.html
    - `Internals <https://www.postgresql.org/docs/11/internals.html>`_
    - `Database File Layout <https://www.postgresql.org/docs/11/storage-file-layout.html>`_
    - `System Columns <https://www.postgresql.org/docs/11/ddl-system-columns.html>`_
    - `Genetic Query Optimizer <https://www.postgresql.org/docs/11/geqo.html>`_
    - `Parallel Query <https://www.postgresql.org/docs/11/parallel-query.html>`_
    - `Performance Tips <https://www.postgresql.org/docs/11/performance-tips.html>`_

- External
    - http://www.interdb.jp/pg/
    - https://momjian.us/main/presentations/internals.html
    - https://brandur.org/postgres-atomicity

Indexes
=======

Show Index Properties
---------------------

.. code-block:: sql

    -- Show all index types.
    SELECT oid, amname FROM pg_am;

    -- Show index properties.
    -- pg_indexam_has_property(am_oid, prop_name)
    SELECT amname,
           pg_indexam_has_property(oid, 'can_order') as can_order,
           pg_indexam_has_property(oid, 'can_unique') as can_unique,
           pg_indexam_has_property(oid, 'can_multi_col') as can_multi_col,
           pg_indexam_has_property(oid, 'can_exclude') as can_exclude,
           pg_indexam_has_property(oid, 'can_include') as can_include
    FROM pg_am;

    -- Show specific index properties.
    -- pg_index_has_property(index_oid, prop_name)
    SELECT indexname, tablename,
           pg_index_has_property(indexname::regclass, 'clusterable') as clusterable,
           pg_index_has_property(indexname::regclass, 'index_scan') as index_scan,
           pg_index_has_property(indexname::regclass, 'bitmap_scan') as bitmap_scan,
           pg_index_has_property(indexname::regclass, 'backward_scan') as backward_scan
    FROM pg_indexes;

    -- Show index column properties.
    -- pg_index_column_has_property(index_oid, column_no, prop_name)
    SELECT indexname,
           pg_index_column_has_property(indexname::regclass, colno, 'asc') as asc,
           pg_index_column_has_property(indexname::regclass, colno, 'desc') as desc,
           pg_index_column_has_property(indexname::regclass, colno, 'nulls_first') as nulls_first,
           pg_index_column_has_property(indexname::regclass, colno, 'nulls_last') as nulls_last,
           pg_index_column_has_property(indexname::regclass, colno, 'orderable') as orderable,
           pg_index_column_has_property(indexname::regclass, colno, 'distance_orderable') as distance_orderable,
           pg_index_column_has_property(indexname::regclass, colno, 'returnable') as returnable,
           pg_index_column_has_property(indexname::regclass, colno, 'search_array') as search_array,
           pg_index_column_has_property(indexname::regclass, colno, 'search_nulls') as search_nulls
    FROM unnest(
        array['members_pkey'],
        array[1]
    ) p(indexname, colno);

References
~~~~~~~~~~

- https://www.postgresql.org/docs/11/functions-info.html
- https://www.postgresql.org/docs/11/indexes-types.html

Index-Related Scanning Techniques
---------------------------------

- ``IndexScan``

    - Example

        .. code-block:: sql

            EXPLAIN
            SELECT * FROM bookings
            WHERE bookid = 1;
            /*
                                            QUERY PLAN
            -------------------------------------------------------------------------------
             Index Scan using bookings_pkey on bookings  (cost=0.28..8.30 rows=1 width=36)
               Index Cond: (bookid = 1)
            */

            EXPLAIN
            SELECT * FROM bookings
            ORDER BY bookid;
            /*
                                                QUERY PLAN
            ------------------------------------------------------------------------------------
             Index Scan using bookings_pkey on bookings  (cost=0.28..149.94 rows=4044 width=36)
            */

- ``IndexOnlyScan`` (covering indexes)

    - Condition

        - ``IndexScan`` is applicable.
        - **AND** requested columns are already in the index.

    - Example

        .. code-block:: sql

            EXPLAIN
            SELECT bookid FROM bookings
            WHERE bookid = 1;
            /*
                                                QUERY PLAN
            -----------------------------------------------------------------------------------
             Index Only Scan using bookings_pkey on bookings  (cost=0.28..8.30 rows=1 width=8)
               Index Cond: (bookid = 1)
            */

    - Note:

        - Covering indexes can be created by adding ``INCLUDE`` in ``CREATE INDEX`` command.
        - The additional data is stored only in the leaf nodes of B-Tree.

- ``BitmapIndexScan``

    - Condition

        - ``IndexScan`` is applicable.
        - **AND** the optimizer predicts ``IndexScan`` will lead to too many duplicated page loads.
            - ``BitmapIndexScan`` builds a bipmap of pages that need to be loaded.

    - Example

        .. code-block:: sql

            SET enable_indexscan=false;

            EXPLAIN
            SELECT memid FROM members
            WHERE memid = 1;
            /*
                                            QUERY PLAN
            ---------------------------------------------------------------------------
             Bitmap Heap Scan on members  (cost=4.15..8.16 rows=1 width=8)
               Recheck Cond: (memid = 1)
               ->  Bitmap Index Scan on members_pkey  (cost=0.00..4.15 rows=1 width=0)
                     Index Cond: (memid = 1)
            */

Multicolumn Indexes
-------------------

With multicolumn index, data is sorted from left to right (like string sorting). For example

+---+---+---+
| a | b | c |
+===+===+===+
| 1 | 9 | 5 |
+---+---+---+
| 2 | 0 | 1 |
+---+---+---+
| 2 | 5 | 6 |
+---+---+---+
| 2 | 9 | 7 |
+---+---+---+
| 3 | 3 | 6 |
+---+---+---+
| 5 | 4 | 8 |
+---+---+---+
| 5 | 4 | 9 |
+---+---+---+
| 5 | 5 | 4 |
+---+---+---+

References
~~~~~~~~~~

- https://www.postgresql.org/docs/11/indexes-multicolumn.html


Indexes On Expressions
----------------------

If the criteria in query use functions that are different from the indexing expression, index scan cannot be utilized.

.. code-block:: sql

    \d members

    SET enable_seqscan=false;

    EXPLAIN
    SELECT * FROM members WHERE telephone = '1';
    /*
                                    QUERY PLAN
    --------------------------------------------------------------------------------
     Index Scan using telephone_idx on members  (cost=0.14..8.15 rows=1 width=1438)
       Index Cond: ((telephone)::text = '1'::text)
    */

    EXPLAIN
    SELECT * FROM members WHERE lower(telephone) = '1';
    /* Seq Scan is picked because of lower function.
                                    QUERY PLAN
    ------------------------------------------------------------------------------
     Seq Scan on members  (cost=10000000000.00..10000000001.47 rows=1 width=1438)
       Filter: (lower((telephone)::text) = '1'::text)
    */

    EXPLAIN
    SELECT * FROM members WHERE lower(firstname) = '1';
    /*
                                    QUERY PLAN
    --------------------------------------------------------------------------------
     Index Scan using firstname_idx on members  (cost=0.14..8.15 rows=1 width=1438)
       Index Cond: (lower((firstname)::text) = '1'::text)
    */

    EXPLAIN
    SELECT * FROM members WHERE firstname = '1';
    /* Seq Scan is picked because index expression uses lower function.
                                      QUERY PLAN
    ------------------------------------------------------------------------------
     Seq Scan on members  (cost=10000000000.00..10000000001.39 rows=1 width=1438)
       Filter: ((firstname)::text = '1'::text)
    */

References
~~~~~~~~~~

- https://www.postgresql.org/docs/11/indexes-expressional.html

References
----------

- https://www.postgresql.org/docs/11/indexes.html
- https://use-the-index-luke.com
- https://habr.com/en/company/postgrespro/blog/441962/

Isolation Levels
================

``READ UNCOMMITTED``
--------------------

Not possible in postgresql.

``READ COMMITTED``
------------------

Short Conflicting Transaction
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. code-block:: sql

    /******************************************************************************************************************************
    TxA                                                             TxB
    *******************************************************************************************************************************/
    /* 1 */
    BEGIN;

    /* 2 */
    SELECT xmin, cmin, xmax, cmax, ctid,
           slots, bookid
    FROM bookings
    WHERE bookid < 4000
    ORDER BY bookid DESC LIMIT 2;
    --  xmin | cmin | xmax | cmax |  ctid   | slots | bookid
    -- ------+------+------+------+---------+-------+--------
    --   577 |    0 |    0 |    0 | (33,39) |     2 |   3999 <===
    --   577 |    0 |    0 |    0 | (33,38) |     2 |   3998
    -- (2 rows)

                                                                    /* 3 */
                                                                    DELETE FROM bookings WHERE bookid = 3999;

    /* 4 */
    SELECT xmin, cmin, xmax, cmax, ctid,
           slots, bookid
    FROM bookings
    WHERE bookid < 4000
    ORDER BY bookid DESC LIMIT 2;
    --  xmin | cmin | xmax | cmax |  ctid   | slots | bookid
    -- ------+------+------+------+---------+-------+--------
    --   577 |    0 |    0 |    0 | (33,38) |     2 |   3998 <===
    --   577 |    0 |    0 |    0 | (33,37) |     2 |   3997
    -- (2 rows)

    /* 5 */
    UPDATE bookings SET slots = 9 WHERE bookid = 3999;
    -- UPDATE 0

    /* 6 */
    ROLLBACK;

Long Conflicting Transaction
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. code-block:: sql

    /******************************************************************************************************************************
    TxA                                                             TxB
    *******************************************************************************************************************************/
    /* 1 */
    BEGIN;

                                                                    /* 2 */
                                                                    BEGIN;

    /* 3 */
    SELECT xmin, cmin, xmax, cmax, ctid,
           slots, bookid
    FROM bookings
    WHERE bookid < 3500
    ORDER BY bookid DESC LIMIT 2;
    --  xmin | cmin | xmax | cmax |  ctid   | slots | bookid
    -- ------+------+------+------+---------+-------+--------
    --   577 |    0 |    0 |    0 | (29,19) |     3 |   3499 <===
    --   577 |    0 |    0 |    0 | (29,18) |     3 |   3498
    -- (2 rows)

                                                                    /* 4 */
                                                                    DELETE FROM bookings WHERE bookid = 3499;

    /* 5 */
    SELECT xmin, cmin, xmax, cmax, ctid,
           slots, bookid
    FROM bookings
    WHERE bookid < 3500
    ORDER BY bookid DESC LIMIT 2;
    --  xmin | cmin | xmax | cmax |  ctid   | slots | bookid
    -- ------+------+------+------+---------+-------+--------
    --   577 |    0 |  580 |    0 | (29,19) |     3 |   3499 <===
    --   577 |    0 |    0 |    0 | (29,18) |     3 |   3498
    -- (2 rows)

    /* 6 */
    UPDATE bookings SET slots = 9 WHERE bookid = 3499;
    -- Wait...

                                                                    /* 7 */
                                                                    COMMIT;

    -- ... until TxB is done.
    -- UPDATE 0

    /* 8 */
    SELECT xmin, cmin, xmax, cmax, ctid,
           slots, bookid
    FROM bookings
    WHERE bookid < 3500
    ORDER BY bookid DESC LIMIT 2;
    --  xmin | cmin | xmax | cmax |  ctid   | slots | bookid
    -- ------+------+------+------+---------+-------+--------
    --   577 |    0 |    0 |    0 | (29,18) |     3 |   3498 <===
    --   577 |    0 |    0 |    0 | (29,17) |     3 |   3497
    -- (2 rows)

    /* 9 */
    ROLLBACK;

.. code-block:: sql

    /******************************************************************************************************************************
    TxA                                                             TxB
    *******************************************************************************************************************************/
    /* 1 */
    BEGIN;

                                                                    /* 2 */
                                                                    BEGIN;

    /* 3 */
    SELECT xmin, cmin, xmax, cmax, ctid,
           slots, bookid
    FROM bookings
    WHERE bookid < 3000
    ORDER BY bookid DESC LIMIT 2;
    --  xmin | cmin | xmax | cmax |   ctid   | slots | bookid
    -- ------+------+------+------+----------+-------+--------
    --   577 |    0 |    0 |    0 | (24,119) |     2 |   2999 <===
    --   577 |    0 |    0 |    0 | (24,118) |     2 |   2998
    -- (2 rows)

                                                                    /* 4 */
                                                                    DELETE FROM bookings WHERE bookid = 2999;

    /* 5 */
    SELECT xmin, cmin, xmax, cmax, ctid,
           slots, bookid
    FROM bookings
    WHERE bookid < 3000
    ORDER BY bookid DESC LIMIT 2;
    --  xmin | cmin | xmax | cmax |   ctid   | slots | bookid
    -- ------+------+------+------+----------+-------+--------
    --   577 |    0 |  582 |    0 | (24,119) |     2 |   2999 <===
    --   577 |    0 |    0 |    0 | (24,118) |     2 |   2998
    -- (2 rows)

                                                                    /* 6 */
                                                                    ROLLBACK;

    /* 7 */
    SELECT xmin, cmin, xmax, cmax, ctid,
           slots, bookid
    FROM bookings
    WHERE bookid < 3000
    ORDER BY bookid DESC LIMIT 2;
    --  xmin | cmin | xmax | cmax |   ctid   | slots | bookid
    -- ------+------+------+------+----------+-------+--------
    --   577 |    0 |  582 |    0 | (24,119) |     2 |   2999 <===
    --   577 |    0 |    0 |    0 | (24,118) |     2 |   2998
    -- (2 rows)

    /* 8 */
    ROLLBACK;

    /* 9 */
    SELECT xmin, cmin, xmax, cmax, ctid,
           slots, bookid
    FROM bookings
    WHERE bookid < 3000
    ORDER BY bookid DESC LIMIT 2;
    --  xmin | cmin | xmax | cmax |   ctid   | slots | bookid
    -- ------+------+------+------+----------+-------+--------
    --   577 |    0 |  582 |    0 | (24,119) |     2 |   2999 <===
    --   577 |    0 |    0 |    0 | (24,118) |     2 |   2998
    -- (2 rows)

    /* 10 */
    VACUUM FULL;

    /* 11 */
    SELECT xmin, cmin, xmax, cmax, ctid,
           slots, bookid
    FROM bookings
    WHERE bookid < 3000
    ORDER BY bookid DESC LIMIT 2;
    --  xmin | cmin | xmax | cmax |   ctid   | slots | bookid
    -- ------+------+------+------+----------+-------+--------
    --   577 |    0 |    0 |    0 | (24,119) |     2 |   2999 <===
    --   577 |    0 |    0 |    0 | (24,118) |     2 |   2998
    -- (2 rows)

``REPEATABLE READ``
-------------------

.. code-block:: sql

    /******************************************************************************************************************************
    TxA                                                             TxB
    *******************************************************************************************************************************/
    /* 1 */
    BEGIN TRANSACTION ISOLATION LEVEL REPEATABLE READ;

    /* 2 */
    SELECT xmin, cmin, xmax, cmax, ctid,
           slots, bookid
    FROM bookings
    WHERE bookid < 2500
    ORDER BY bookid DESC LIMIT 2;
    --  xmin | cmin | xmax | cmax |  ctid   | slots | bookid
    -- ------+------+------+------+---------+-------+--------
    --   577 |    0 |    0 |    0 | (20,99) |     2 |   2499 <===
    --   577 |    0 |    0 |    0 | (20,98) |     2 |   2498
    -- (2 rows)

                                                                    /* 3 */
                                                                    DELETE FROM bookings WHERE bookid = 2499;

    /* 4 */
    SELECT xmin, cmin, xmax, cmax, ctid,
           slots, bookid
    FROM bookings
    WHERE bookid < 2500
    ORDER BY bookid DESC LIMIT 2;
    --  xmin | cmin | xmax | cmax |  ctid   | slots | bookid
    -- ------+------+------+------+---------+-------+--------
    --   577 |    0 |  659 |    0 | (20,99) |     2 |   2499 <===
    --   577 |    0 |    0 |    0 | (20,98) |     2 |   2498
    -- (2 rows)

    /* 5 */
    UPDATE bookings SET slots = 9 WHERE bookid = 2499;
    -- ERROR:  could not serialize access due to concurrent update

    /* 6 */
    ROLLBACK;

``SERIALIZABLE``
----------------

I don't care about this one.

References
----------

- https://www.postgresql.org/docs/11/transaction-iso.html
- http://www.interdb.jp/pg/pgsql05.html

Security
========

References
----------

- https://info.enterprisedb.com/rs/069-ALB-339/images/security-best-practices-for-postgres.pdf
- https://momjian.us/main/writings/pgsql/securing.pdf