```
neighbour.c::neigh_sysctl_template

net/ipv6/ndisc.c::ndisc_init
    neigh_table_init
    neigh_sysctl_register("ipv6", ndisc_ifinfo_sysctl_change)
        register_net_sysctl


net/ipv6/ndisc.c::ndisc_router_discovery
    // Update Reachable Time and Retrans Timer
    __neigh_lookup


net/ipv6/icmp.c::icmpv6_rcv
    net/ipv6/ndisc.c::ndisc_rcv


include/net/neighbour.h::__neigh_lookup
    neigh_create
        __neigh_create
            neigh_alloc


net/ipv6/ndisc.c::ndisc_rcv
    ndisc_recv_ns
    ndisc_recv_na
    ndisc_recv_rs
    ndisc_router_discovery
    ndisc_redirect_rcv


net/core/neighbour.c::neigh_alloc
    // check gc_thresh -> call neigh_forced_gc
    // alloc new neighbour


net/core/neighbour.c::neigh_periodic_work
    // periodically recompute ReachableTime from random function
    neigh_cleanup_and_release
        __neigh_notify(RTM_DELNEIGH)
        neigh_release
            neigh_destroy
```