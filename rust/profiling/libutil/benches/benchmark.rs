use criterion::criterion_group;
use criterion::criterion_main;
use criterion::Criterion;

fn bench_fib(c: &mut Criterion) {
    c.bench_function("fib 20", |b| b.iter(|| libutil::fib(20)));
}

criterion_group!(benches, bench_fib);
criterion_main!(benches);
