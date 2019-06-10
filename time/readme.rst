.. contents:: Table of Contents

Concerns
========

- Different parts of the system use different time systems.
- Timezone will be changed in the future.
- DST.

  * It will be changed in the future.
  * Different countries have different transition time.

    + ``01:59:59`` + 1s = ``03:00:00``
    + ``23:59:59`` + 1s = ``01:00:00``

  * Time sorting.

- Textual representation is misunderstood by programmers.

  * In ``2019-06-10T17:15:00-08:00``, ``-08:00`` represents time offset to UTC, not timezone.

- User expectations about date time arithmetic.

  * What is the result of ``2019/01/31`` + 1 month? Is it always same as ``2019/01/31`` + 30 days?
  * What is the result of ``2019/02/28`` - 1 month? Is it always same as ``2019/02/28`` - 30 days?

- Time is stored without sufficient information for future usage.

  * Convert local time to UTC, then time zone/DST rules change, cannot convert stored time back to local time easily.

References
==========

- https://www.youtube.com/watch?v=saeKBuPewcU
