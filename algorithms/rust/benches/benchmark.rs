use criterion::criterion_group;
use criterion::criterion_main;
use criterion::Criterion;

fn tadm_c4_fn_10(c: &mut Criterion) {
    c.bench_function("tadm::c4::fn_10", |b| {
        b.iter(|| rust::tadm::c4::fn_10(&mut [1, 2, 3, 4], 1, 2))
    });
}

criterion_group!(benches, tadm_c4_fn_10);
criterion_main!(benches);
