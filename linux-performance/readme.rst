.. contents:: Table of Contents
.. section-numbering::

Concepts
========

+-------------+---------------------------------------------------------------------------------------+-------------------------------------+
| Measurement | Definition                                                                            | Unit                                |
+=============+=======================================================================================+=====================================+
| Latency     | How long to accomplish a request or operation.                                        | milliseconds/operation              |
+-------------+---------------------------------------------------------------------------------------+-------------------------------------+
| Rate        | An operation or request rate per second.                                              | operations/sec // read-requests/sec |
+-------------+---------------------------------------------------------------------------------------+-------------------------------------+
| Throughput  |                                                                                       | bits/sec // bytes/sec               |
+-------------+---------------------------------------------------------------------------------------+-------------------------------------+
| Utilization | The average time that the resource was busy servicing work.                           | %                                   |
+-------------+---------------------------------------------------------------------------------------+-------------------------------------+
| Saturation  | The degree to which the resource has extra work which it can't service, often queued. | length of queued operations         |
+-------------+---------------------------------------------------------------------------------------+-------------------------------------+
| Cost        | The price/performance ratio.                                                          |                                     |
+-------------+---------------------------------------------------------------------------------------+-------------------------------------+

Quick/Conventional Tools
========================

+------------+-----------------------+---------------------+---------------+------------------+------------+
|            | Overview              | Per CPU             | Per process   | Per device       | Per domain |
+============+=======================+=====================+===============+==================+============+
| CPU/Load   | ``uptime``            | ``mpstat -P ALL 1`` | ``pidstat 1`` |                  |            |
+------------+-----------------------+---------------------+---------------+------------------+------------+
| Memory     | ``vmstat 1``          |                     |               |                  |            |
|            | ``free -m``           |                     |               |                  |            |
+------------+-----------------------+---------------------+---------------+------------------+------------+
| Disk IO    |                       |                     | ``iotop``     | ``iostat -xz 1`` |            |
+------------+-----------------------+---------------------+---------------+------------------+------------+
| Network IO | ``sar -n TCP,ETCP 1`` |                     | ``nethogs``   | ``sar -n DEV 1`` | ``iftop``  |
+------------+-----------------------+---------------------+---------------+------------------+------------+

``uptime``
----------

::

                                                  #     #     #

                                                  load1 load5 load15
                                                  |     |     |
                                                  v     v     v
    20:20:00  up   0:26,  5 users,  load average: 2.01, 1.84, 1.64

- Is system load increasing, decreasing or stable?

  * Increasing/Decreasing -> We are late/not late to analyze the system while its symptoms start to appear.
  * Stable -> Let's get to work.

System Logs
-----------

::

    sudo dmesg --follow --ctime
    sudo journalctl --boot --follow

- Is there any error indicated in the log?

``vmstat 1``
------------

::

     #  #                                                                 #  #  #  #

     +- The number of runnable processes (running or waiting for run time).
     |  +- The number of processes in uninterruptible sleep.             +~~~~~~~~~~~~~ user sys idle iowait stolen-time
     |  |                                                                |
     v  v                                                                v
    procs  -----------memory----------  ---swap-- -----io---- --system-- ------cpu-----
     r  b   swpd   free   buff  cache    si   so     bi    bo   in    cs us sy id wa st <- first line is summary since boot
     3  2  16640 277016   5572 5184688    0    8    390   902  964   387 24 10 65  1  0
     0  0  16640 243296   5572 5218084    0    0  32248  2668 6794 24668 14 17 56 13  0
     1  0  16640 241180   5572 5218172    0    0     12     0 2403 15211 19  8 73  0  0

- Is the number of runnable processes (r) higher than the number of cores?

  * Yes means CPU is saturated, there are more jobs than it can serve.

- How is CPU time distributed over user/system/idle/iowait?

  * This can shed some light on where the bottleneck is (application code, kernel code, disk/network latency).

``S_COLORS=auto mpstat -P ALL 1``
---------------------------------

::

                           #               #       #       #       #

                                                           +- Percentage of time spent to service hardware interrupts.
                                                           |       +- Percentage of time spent to service software interrupts.
                                                           |       |
                                                           v       v
    08:49:51 PM  CPU    %usr   %nice    %sys %iowait    %irq   %soft  %steal  %guest  %gnice   %idle
    08:49:52 PM  all   17.14    0.00    5.12    0.00    0.00    0.00    0.00    0.00    0.00   77.75
    08:49:52 PM    0   20.20    0.00    5.05    0.00    0.00    0.00    0.00    0.00    0.00   74.75
    08:49:52 PM    1   14.43    0.00    6.19    0.00    0.00    0.00    0.00    0.00    0.00   79.38
    08:49:52 PM    2   16.67    0.00    5.21    0.00    0.00    0.00    0.00    0.00    0.00   78.12
    08:49:52 PM    3   17.17    0.00    4.04    0.00    0.00    0.00    0.00    0.00    0.00   78.79

    08:49:52 PM  CPU    %usr   %nice    %sys %iowait    %irq   %soft  %steal  %guest  %gnice   %idle
    08:49:53 PM  all   14.77    0.26    6.48    0.52    0.00    0.00    0.00    0.00    0.00   77.98
    08:49:53 PM    0   12.37    0.00    9.28    0.00    0.00    0.00    0.00    0.00    0.00   78.35
    08:49:53 PM    1   16.67    1.04    4.17    2.08    0.00    0.00    0.00    0.00    0.00   76.04
    08:49:53 PM    2   15.15    0.00    8.08    0.00    0.00    0.00    0.00    0.00    0.00   76.77
    08:49:53 PM    3   14.89    0.00    4.26    0.00    0.00    0.00    0.00    0.00    0.00   80.85

- Do the high metrics spread across all CPUs equally?

  * Yes means it is possibly caused by multiple threads (from single process or multiple ones).
  * No means it is maybe caused by only one process.

- Is ``%irq`` or ``%soft`` high?

  * Yes means the culprit can be network, disk, or other device drivers.

``S_COLORS=auto pidstat 1``
---------------------------

::

                                      #       #               #       #

    08:50:57 PM   UID       PID    %usr %system  %guest   %wait    %CPU   CPU  Command
    08:50:58 PM     0      1913    0.99    0.00    0.00    0.00    0.99     0  dockerd
    08:50:58 PM   452      2124    0.00    1.98    0.00    0.00    1.98     3  netdata
    08:50:58 PM     0      2149    2.97    0.99    0.00    0.00    3.96     2  X
    08:50:58 PM   452      2240    0.99    1.98    0.00    0.00    2.97     0  apps.plugin
    08:50:58 PM  1000      3183    3.96    3.96    0.00    0.00    7.92     0  kwin_x11
    08:50:58 PM  1000      3188    0.99    0.00    0.00    0.00    0.99     1  plasmashell
    08:50:58 PM  1000      3226    0.00    0.99    0.00    0.00    0.99     2  pulseaudio
    08:50:58 PM  1000      3767   12.87    3.96    0.00    0.00   16.83     3  firefox-bin
    08:50:58 PM  1000      4056    0.99    0.99    0.00    0.00    1.98     0  WebExtensions
    08:50:58 PM  1000      4298    1.98    0.00    0.00    0.00    1.98     3  RDD Process
    08:50:58 PM  1000      4603   23.76    7.92    0.00    0.00   31.68     2  Web Content
    08:50:58 PM  1000     14321    1.98    1.98    0.00    2.97    3.96     0  http.so
    08:50:58 PM  1000     14343    1.98    1.98    0.00    0.99    3.96     3  http.so
    08:50:58 PM  1000     14344    0.99    1.98    0.00    0.99    2.97     3  http.so
    08:50:58 PM  1000     14581    1.98    0.99    0.00    0.00    2.97     1  pidstat

- Are the metrics high for any particular process?

  * Yes means that process can be the offender.

``S_COLORS=auto iostat -xz 1``
------------------------------

::

                        #         #                       #                #         #                       #                                                                   #      #

                        +- Completed read/write requests/sec. -------------+                           The average queue length of the requests that were issued to the device. -+
                        |         +- Read/write speed. --------------------|---------+                 Values greater than one can be evidence of saturation.                    |      +- Device utilization.
                        |         |                                        |         |                                                                                           |      |
                        v         v                                        v         v                                                                                           v      v
    Device            r/s     rkB/s   rrqm/s  %rrqm r_await rareq-sz     w/s     wkB/s   wrqm/s  %wrqm w_await wareq-sz     d/s     dkB/s   drqm/s  %drqm d_await dareq-sz  aqu-sz  %util
    dm-0            23.77    807.39     0.00   0.00    1.98    33.97   59.90   2790.44     0.00   0.00   55.91    46.59    0.00      0.00     0.00   0.00    0.00     0.00    3.40   5.06
    dm-1             1.19    352.29     0.00   0.00    3.16   295.66    0.20     74.65     0.00   0.00   70.47   369.62    0.00      0.00     0.00   0.00    0.00     0.00    0.02   0.15
    dm-2             0.31      1.95     0.00   0.00   14.36     6.28    5.89     23.54     0.00   0.00   97.34     4.00    0.00      0.00     0.00   0.00    0.00     0.00    0.58   0.07
    sda             24.68    848.66     0.88   3.46    1.80    34.39   59.30   2813.98     6.48   9.85    4.30    47.45    0.00      0.00     0.00   0.00    0.00     0.00    0.24   5.08
    sdb              1.18    353.58     0.07   5.33    2.51   300.17    0.20     74.65     0.00   1.87    4.08   376.65    0.00      0.00     0.00   0.00    0.00     0.00    0.00   0.15
                                                          ^                                                  ^
                                                          |                                                  |
                                                          +- The average time for the I/O in milliseconds. --+
                                                             This is the time that the application suffers, as
                                                             it includes both time queued and time being serviced.

- Are the transmit rates high?
- Is ``r_await`` or ``w_await`` high?

  * Yes means the processes that issued those read/write requests are suffering.

``free -m``
-----------

::

                  total        used        free      shared  buff/cache   available
    Mem:           7856        2390         345         191        5121        4975
    Swap:          7843          17        7826

- Is ``available`` low?
- Is swap memory used a lot?

``S_COLORS=auto sar -n DEV 1``
------------------------------

::

                                  #         #         #         #                                       #

    08:52:08 PM     IFACE   rxpck/s   txpck/s    rxkB/s    txkB/s   rxcmp/s   txcmp/s  rxmcst/s   %ifutil
    08:52:09 PM br-3589b5fc4986      0.00      0.00      0.00      0.00      0.00      0.00      0.00      0.00
    08:52:09 PM   docker0      0.00      0.00      0.00      0.00      0.00      0.00      0.00      0.00
    08:52:09 PM    virbr0      0.00      0.00      0.00      0.00      0.00      0.00      0.00      0.00
    08:52:09 PM virbr0-nic      0.00      0.00      0.00      0.00      0.00      0.00      0.00      0.00
    08:52:09 PM       em1      0.00      0.00      0.00      0.00      0.00      0.00      0.00      0.00
    08:52:09 PM    wlp2s0      0.00      0.00      0.00      0.00      0.00      0.00      0.00      0.00
    08:52:09 PM        lo      0.00      0.00      0.00      0.00      0.00      0.00      0.00      0.00

``S_COLORS=auto sar -n TCP,ETCP 1``
-----------------------------------

::

                        #         #

                        +- Out connections.
                        |         +- Accepted connections.
                        |         |
                        v         v
    08:52:57 PM  active/s passive/s    iseg/s    oseg/s
    08:52:58 PM      0.00      0.00      0.00      0.00

    08:52:57 PM  atmptf/s  estres/s retrans/s isegerr/s   orsts/s
    08:52:58 PM      0.00      0.00      0.00      0.00      0.00
                                            ^
                                            |
                                            +- Number of TCP retransmits

                                            #

- Is connection creation/acceptance rate high?
- Is retransmit rate high?

  * Yes means the receiving end is having problems.

``top``/``htop``/``atop``
-------------------------

BCC Tools
=========

``execsnoop``
-------------

::

    PCOMM            PID    PPID   RET ARGS
    sleep            17947  17868    0 /usr/bin/sleep 0

``opensnoop``
-------------

::

    PID    COMM               FD ERR PATH
    3543   ksysguardd         12   0 /proc/cpuinfo
    3543   ksysguardd         12   0 /sys/bus/cpu/devices/cpu0/cpufreq/scaling_cur_freq
    3543   ksysguardd         12   0 /sys/bus/cpu/devices/cpu1/cpufreq/scaling_cur_freq
    3543   ksysguardd         12   0 /sys/bus/cpu/devices/cpu2/cpufreq/scaling_cur_freq
    3543   ksysguardd         12   0 /sys/bus/cpu/devices/cpu3/cpufreq/scaling_cur_freq
    3543   ksysguardd         12   0 /proc/stat
    3543   ksysguardd         12   0 /proc/vmstat
    3543   ksysguardd         12   0 /proc/meminfo
    17725  bash                3   0 /proc/uptime

``[ext4|btrfs|xfs|zfs]slower``
------------------------------

::

    Tracing btrfs operations slower than 10 ms
    TIME     COMM           PID    T BYTES   OFF_KB   LAT(ms) FILENAME
    21:56:11 DOMCacheThread 3767   S 0       0          12.93 caches.sqlite-wal
    21:56:13 LS Thread      3767   S 0       0          55.37 data.sqlite

``biolatency``
--------------

::

    Tracing block device I/O... Hit Ctrl-C to end.
    ^C
         usecs               : count     distribution
             0 -> 1          : 0        |                                        |
             2 -> 3          : 0        |                                        |
             4 -> 7          : 0        |                                        |
             8 -> 15         : 0        |                                        |
            16 -> 31         : 0        |                                        |
            32 -> 63         : 11       |***************                         |
            64 -> 127        : 29       |****************************************|
           128 -> 255        : 8        |***********                             |
           256 -> 511        : 0        |                                        |
           512 -> 1023       : 18       |************************                |
          1024 -> 2047       : 4        |*****                                   |
          2048 -> 4095       : 22       |******************************          |
          4096 -> 8191       : 3        |****                                    |

``biosnoop``
------------

::

    TIME(s)     COMM           PID    DISK    T SECTOR     BYTES  LAT(ms)
    0.000000    dmcrypt_write/ 539    sda     W 24149784   32768     3.28
    0.000148    dmcrypt_write/ 539    sda     W 24159776   98304     3.41

``cachestat``
-------------

::

    HITS   MISSES  DIRTIES HITRATIO   BUFFERS_MB  CACHED_MB
       5        0        0  100.00%            5       4400
      82        0        0  100.00%            5       4399

``tcpconnect``
--------------

::

    PID    COMM         IP SADDR            DADDR            DPORT
    3767   Socket Threa 4  192.168.1.13     34.217.232.36    443 
    3767   Socket Threa 4  192.168.1.13     172.217.31.228   443

``tcpaccept``
-------------

::

    PID     COMM         IP RADDR            RPORT LADDR            LPORT
    1947    sshd         4  127.0.0.1        39068 127.0.0.1        22

``tcpretrans``
--------------

::

    Tracing retransmits ... Hit Ctrl-C to end
    TIME     PID    IP LADDR:LPORT          T> RADDR:RPORT          STATE
    01:55:05 0      4  10.153.223.157:22    R> 69.53.245.40:34619   ESTABLISHED

``runqlat``
-----------

::

    Tracing run queue latency... Hit Ctrl-C to end.
    ^C
         usecs               : count     distribution
             0 -> 1          : 1391     |****                                    |
             2 -> 3          : 9865     |****************************            |
             4 -> 7          : 13673    |****************************************|
             8 -> 15         : 6253     |******************                      |
            16 -> 31         : 4530     |*************                           |
            32 -> 63         : 5854     |*****************                       |
            64 -> 127        : 796      |**                                      |
           128 -> 255        : 286      |                                        |
           256 -> 511        : 168      |                                        |
           512 -> 1023       : 85       |                                        |
          1024 -> 2047       : 43       |                                        |
          2048 -> 4095       : 25       |                                        |
          4096 -> 8191       : 7        |                                        |
          8192 -> 16383      : 1        |                                        |
         16384 -> 32767      : 0        |                                        |
         32768 -> 65535      : 0        |                                        |
         65536 -> 131071     : 1        |                                        |

``profile``
-----------

::

    QCoreApplication::translate(char const*, char const*, char const*, int)
    [unknown]
    -                http.so (18702)
        2

    b'finish_task_switch'
    b'finish_task_switch'
    b'__schedule'
    b'schedule'
    b'schedule_hrtimeout_range_clock'
    b'poll_schedule_timeout.constprop.0'
    b'do_sys_poll'
    b'__x64_sys_ppoll'
    b'do_syscall_64'
    b'entry_SYSCALL_64_after_hwframe'
    ppoll
    -                http.so (17150)
        3

Process Analysis
================

- Build flamegraphs for on CPU/off CPU

  * ``offcputime`` and ``profile`` from BCC tools.

- Collect hardware stats (LLC, TLB, branch prediction...)

  * ``perf stat``

- Collect process internal stats using USDT::

    $ tplist -l /usr/lib/postgresql/11/bin/postgres
    /usr/lib/postgresql/11/bin/postgres postgresql:clog__checkpoint__start
    /usr/lib/postgresql/11/bin/postgres postgresql:clog__checkpoint__done
    /usr/lib/postgresql/11/bin/postgres postgresql:multixact__checkpoint__start
    /usr/lib/postgresql/11/bin/postgres postgresql:multixact__checkpoint__done
    /usr/lib/postgresql/11/bin/postgres postgresql:subtrans__checkpoint__start
    /usr/lib/postgresql/11/bin/postgres postgresql:subtrans__checkpoint__done
    ...

References
==========

- `BPF Performance Tools <http://www.brendangregg.com/bpf-performance-tools-book.html>`__
