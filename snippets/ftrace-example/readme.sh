# Offical document.
#   https://www.kernel.org/doc/Documentation/trace/ftrace.txt

cd /sys/kernel/debug/tracing

# Display help.
#   This is very important, please read it first.
cat README

############################################## Example : Trace write function. #

# Disable tracing.
echo 0 > tracing_on

# Clear trace buffer.
echo > trace

# Enable function graph.
echo function_graph > current_tracer

# Display process that calls the monitored function (unnecessary in case we only monitor one process).
echo funcgraph-proc > trace_options

# Set monitored function.
# echo SyS_write > set_ftrace_filter
echo SyS_write > set_graph_function

# Set process to monitor.
echo <process-pid> > set_ftrace_pid

# Enable tracing back on.
echo 1 > tracing_on

# ... waiting ...

# Disable tracing and print out log.
echo 0 > tracing_on
cat trace
