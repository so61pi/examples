.. contents:: Table of Contents

Using ``trace-cmd``
===================

- ``trace-cmd`` is a front end of ``ftrace``

List Options
------------

List Available Tracers
~~~~~~~~~~~~~~~~~~~~~~

.. code-block:: shell

    trace-cmd list -t | tr ' ' '\n'
    # blk
    # function_graph
    # wakeup_dl
    # wakeup_rt
    # wakeup
    # function
    # nop

List Available Functions To Filter On By Regex
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. code-block:: shell

    trace-cmd list -f '^vfs_re'
    # vfs_read
    # vfs_readv
    # vfs_readlink
    # vfs_rename
    # vfs_removexattr

List Plugin Options
~~~~~~~~~~~~~~~~~~~

.. code-block:: shell

    trace-cmd list -O
    #   plugin:       ftrace
    #   option:       parent
    #     desc:       Print parent of functions for function events
    #      set:       0
    # ------------
    #   plugin:       ftrace
    #   option:       indent
    #     desc:       Try to show function call indents, based on parents
    #      set:       1
    # ============
    #   plugin:       fgraph
    #   option:       tailprint
    #     desc:       Print function name at function exit in function graph
    #      set:       0
    # ------------
    #   plugin:       fgraph
    #   option:       depth
    #     desc:       Show the depth of each entry
    #      set:       0

Trace
-----

.. code-block:: shell

    trace-cmd reset
    trace-cmd record -p function_graph -g vfs_read -F cat /dev/null

    # -g set graph function
    #    write function name that we want to trace to set_graph_function
    # -n do not trace function
    #    write function name that we do NOT want to trace to set_ftrace_notrace
    #    used in case we don't want to trace some internal functions called by the function being traced

View Report
-----------

.. code-block:: shell

    trace-cmd report
    # trace-cmd report -O fgraph:tailprint

References
----------

- https://lwn.net/Articles/410200/
- https://elinux.org/Ftrace

Using ``ftrace`` Manually
=========================

Setup
-----

.. code-block:: shell

    # Disable tracing.
    echo 0 > /sys/kernel/debug/tracing/tracing_on

    # Clear trace buffer.
    echo > /sys/kernel/debug/tracing/trace

    # Enable function graph.
    # To get available tracers, use `cat /sys/kernel/debug/tracing/available_tracers`.
    echo function_graph > /sys/kernel/debug/tracing/current_tracer

    # Display process that calls the monitored function (unnecessary in case we only monitor one process).
    # To get available options, use `cat /sys/kernel/debug/tracing/trace_options`.
    echo funcgraph-proc > /sys/kernel/debug/tracing/trace_options

Trace
-----

.. code-block:: shell

    # Set monitored function.
    # To get available functions, use `cat available_filter_functions`.
    echo vfs_write > /sys/kernel/debug/tracing/set_graph_function

    # Set process to monitor.
    echo <process-pid> > /sys/kernel/debug/tracing/set_ftrace_pid

    # Enable tracing back on.
    echo 1 > /sys/kernel/debug/tracing/tracing_on

    # ... waiting ...

    # Disable tracing and print out log.
    echo 0 > /sys/kernel/debug/tracing/tracing_on
    cat trace

References
----------

- ``sudo less /sys/kernel/debug/tracing/README``
- https://www.kernel.org/doc/Documentation/trace/ftrace.txt
- https://lwn.net/Articles/370423/
