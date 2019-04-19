.. contents::


Softwares
=========

- postgresql
    - Tag: ``REL_11_2``
    - Hash: ``6cd404b344f7e27f4d64555bb133f18a758fe851``


Query execution flow
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


Logging/Debug configuration options
===================================

Common options
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


Show Or Set Option
------------------

.. code-block:: sql

    show <option>;

    set <option> to <value>;
    set <option> to default;


Enable Logging Internal Backend Data Flow
-----------------------------------------

.. code-block:: sql

    set debug_print_parse to 'on';
    set debug_print_rewritten to 'on';
    set debug_print_plan to 'on';
    set debug_pretty_print to 'on';

- Log file is placed in ``/var/lib/pgsql/data/log/``


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

- Bitcode file is placed in ``/var/lib/pgsql/data/``, to decompile bitcode or print CFG, use

    .. code-block:: shell

        llvm-dis file.bc
        opt -dot-cfg file.bc


Make Configuration Persistent
-----------------------------

#. View config file location (default location is ``/var/lib/pgsql/data/postgresql.conf``)

    .. code-block:: sql
    
        show config_file;

#. Make changes in that file, then restart postgres service


References
----------

- postgresql/src/backend/utils/misc/guc.c
- https://llvm.org/docs/LangRef.html


References
==========

- https://www.postgresql.org/docs/11/acronyms.html
