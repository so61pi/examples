.. contents:: Table of Contents

UTC/UT/TAI
==============

TAI - International Atomic Time
-------------------------------

    The International System of Units (SI) defines one second as the time it takes a Cesium-133 atom at the ground state to oscillate exactly 9,192,631,770 times.

    --- https://www.timeanddate.com/time/international-atomic-time.html

We can think TAI as an absolute way to measure time (not in the sense of any physics theory). It doesn't depend on earth or sun's movements.

UT/UT1 - Universal Time
-----------------------

In UT, a day is defined as the amount of time for the Earth to complete a rotation around its axis.

As the Earth's angular velocity is getting slower, and people want to keep the length of a day 24 hours, conceptually a day is a little bit longer than **86400** (24 * 60 * 60) seconds.

You can go to https://www.timeanddate.com/time/earth-rotation.html to see how long currently a day is.

UTC
---

Imagine we have a TAI clock running for a few years, we will realize that it counts more than 86400 seconds a day.

In order to use this clock, and keep the number of seconds in a day stable at 86400, once in a while we add a leap second to it so the next day we are back to having 86400 seconds a day. (The concept is similar to leap years.) Consequently, there will be days when we have **86400 + 1** seconds.

With all that said, we cannot modify TAI to accomondate leap seconds, so people invented a new clock system for this, which is called **Coordinated Universal Time**.

References
----------

- https://www.timeanddate.com/time/international-atomic-time.html
- https://www.timeanddate.com/time/universal-time.html
- https://www.timeanddate.com/time/aboututc.html

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
