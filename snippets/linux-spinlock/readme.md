```
// CONFIG_SMP               [X]
// CONFIG_LOCKDEP           [ ]
// CONFIG_DEBUG_SPINLOCK    [ ]
// CONFIG_PREEMPT_COUNT     [X]
// CONFIG_PREEMPT           [X]

include/linux/spinlock.h::spin_lock
    raw_spin_lock ~ include/linux/spinlock_api_smp.h::_raw_spin_lock ~ __raw_spin_lock
        include/linux/preempt.h::preempt_disable
        include/linux/lockdep.h::spin_acquire ~ lock_acquire_exclusive ~ lock_acquire
        include/linux/lockdep.h::LOCK_CONTENDED(lock, do_raw_spin_trylock, do_raw_spin_lock) ~ include/linux/spinlock.h::do_raw_spin_lock(lock)
            arch/x86/include/asm/spinlock.h::arch_spin_lock
                for (;;)


include/linux/spinlock.h::spin_unlock
    raw_spin_unlock ~ include/linux/spinlock_api_smp.h::_raw_spin_unlock ~ __raw_spin_unlock ~ __raw_spin_unlock
        include/linux/lockdep.h::spin_release ~ lock_release
        include/linux/spinlock.h::do_raw_spin_unlock
            arch/x86/include/asm/spinlock.h::arch_spin_unlock
        include/linux/preempt.h::preempt_enable


include/linux/preempt.h::preempt_disable
    preempt_count_inc ~ preempt_count_add ~ arch/x86/include/asm/preempt.h::__preempt_count_add
        arch/x86/include/asm/percpu.h::raw_cpu_add_4 ~ percpu_add_op
    barrier


include/linux/preempt.h::preempt_enable
    preempt_count_dec
        preempt_count_dec_and_test ~ ???::__preempt_count_dec_and_test
        include/asm-generic/preempt.h::__preempt_schedule ~ kernel/sched/core.c::preempt_schedule
            include/linux/preempt.h::preemptible
            preempt_schedule_common
                include/linux/preempt.h::preempt_disable_notrace
                __schedule
                include/linux/preempt.h::preempt_enable_no_resched_notrace
                include/linux/sched.h::need_resched
```
