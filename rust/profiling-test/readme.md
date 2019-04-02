## Test

```shell
# This runs tests in all our crates (libutil and bin-main).
cargo test
```

## Benchmark

```shell
# This runs benchmarks in all our crates (libutil and bin-main).
cargo bench
```

## Flamegraph

```shell
# Enable perf for non-root users.
sudo sh -c "echo -1 | sudo tee /proc/sys/kernel/perf_event_paranoid"

# Run release binary file bin-main and generate flamegraph.
cargo flamegraph --bin bin-main
```

