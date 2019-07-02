.. contents:: Table of Contents

Concerns
========

- Different parts of a system use different time systems.
- Different parts of a system use unsynced time.

  * Front-end on multiple devices.
  * Back-end on multiple nodes.

- Timezone can be changed in the future.
- DST (DST can be thought of as a timezone change event).

  * DST rules can be changed in the future.
  * Different countries have different transition time.

    + ``01:59:59`` + 1s = ``03:00:00``
    + ``23:59:59`` + 1s = ``01:00:00``

  * Handling of duplicated hour (e.g. time sorting).
  * Handling of missing hour (e.g. cron job).

- Textual representation is misunderstood by programmers.

  * In ``2019-06-10T17:15:00-08:00`` (`RFC-3339 <https://tools.ietf.org/html/rfc3339#section-5.6>`__), ``-08:00`` represents time offset to UTC, not timezone.

- User expectations about date time arithmetic.

  * What is the result of ``2019/01/31`` + 1 month? Is it always same as ``2019/01/31`` + 30 days?
  * What is the result of ``2019/02/28`` - 1 month? Is it always same as ``2019/02/28`` - 30 days?

- Time is stored without sufficient information for future usage.

  * Convert local time to UTC, then time zone/DST rules change, cannot convert stored time back to local time easily.

- Leap seconds.
- System time is adjusted forwards/backwards (due to NTP or human intervention).

References
==========

- https://www.youtube.com/watch?v=saeKBuPewcU
- https://en.wikipedia.org/wiki/Israel_Summer_Time#Summer_Time_Dates
