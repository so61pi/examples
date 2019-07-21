.. contents:: Table of Contents

Content is copied from AMQP specification.

Overview
========

    If we make an analogy with an email system we see that the AMQP concepts are not radical:

    - an AMQP message is analogous to an email message;
    - a message queue is like a mailbox;
    - a consumer is like a mail client that fetches and deletes email;
    - a exchange is like a MTA (mail transfer agent) that inspects email and decides, on the basis of routing keys and tables, how to send the email to one or more mailboxes;
    - a routing key corresponds to an email To: or Cc: or Bcc: address, without the server information (routing is entirely internal to an AMQP server);
    - each exchange instance is like a separate MTA process, handling some email sub-domain, or particular type of email traffic;
    - a binding is like an entry in a MTA routing table.

    The power of AMQP comes from our ability to create queues (mailboxes), exchanges (MTA processes), and bindings (routing entries), at runtime, and to chain these together in ways that go far beyond a simple mapping from "to" address to mailbox name.

Exchange Types
==============

Direct
------

    1. A message queue binds to the exchange using a routing key, ``K``.
    2. A publisher sends the exchange a message with the routing key ``R``.
    3. The message is passed to the message queue if ``K`` = ``R``.

    Note that message queues can bind using any valid routing key value, but most often message queues will bind using their own name as routing key.

    In particular, all message queues **MUST BE** automatically bound to the nameless exchange using the message queue's name as routing key.

Fanout
------

    1. A message queue binds to the exchange with no arguments.
    2. A publisher sends the exchange a message.
    3. The message is passed to the message queue unconditionally.

Topic
-----

    1. A message queue binds to the exchange using a routing pattern, ``P``.
    2. A publisher sends the exchange a message with the routing key ``R``.
    3. The message is passed to the message queue if ``R`` matches ``P``.

    The routing key used for a topic exchange **MUST** consist of zero or more words delimited by dots. Eachword may contain the letters ``A-Z`` and ``a-z`` and digits ``0-9``.

    The routing pattern follows the same rules as the routing key with the addition that ``*`` matches a singleword, and ``#`` matches zero or more words. Thus the routing pattern ``*.stock.#`` matches the routing ``keysusd.stock`` and ``eur.stock.db`` but not ``stock.nasdaq``.

Headers
-------

    1. A message queue is bound to the exchange with a table of arguments containing the headers to be matched for that binding and optionally the values they should hold. The routing key is not used.
    2. A publisher sends a message to the exchange where the ``'headers'`` property contains a table ofnames and values.
    3. The message is passed to the queue if the headers property matches the arguments with which the queue was bound.

    The matching algorithm is controlled by a special bind argument passed as a name value pair in the arguments table. The name of this argument is ``'x-match'``. It can take one of two values, dictating how the rest of the name value pairs in the table are treated during matching:

    - ``'all'`` implies that all the other pairs must match the headers property of a message for that message to berouted (i.e. and AND match)
    - ``'any'`` implies that the message should be routed if any of the fields in the headers property match one ofthe fields in the arguments table (i.e. an OR match)

System
------

    1. A publisher sends the exchange a message with the routing key ``S``.
    2. The system exchange passes this to a system service ``S``.

    System services starting with ``"amq."`` are reserved for AMQP usage.

Class Basic's Properties
========================

+-------------------+-----------+---------------------------------------+
| Name              | Domain    | Short Description                     |
+===================+===========+=======================================+
| content-type      | shortstr  | MIME content type                     |
+-------------------+-----------+---------------------------------------+
| content-encoding  | shortstr  | MIME content encoding                 |
+-------------------+-----------+---------------------------------------+
| headers           | table     | message header field table            |
+-------------------+-----------+---------------------------------------+
| delivery-mode     | octet     | non-persistent (1) or persistent (2)  |
+-------------------+-----------+---------------------------------------+
| priority          | octet     | message priority, 0 to 9              |
+-------------------+-----------+---------------------------------------+
| correlation-id    | shortstr  | application correlation identifier    |
+-------------------+-----------+---------------------------------------+
| reply-to          | shortstr  | address to reply to                   |
+-------------------+-----------+---------------------------------------+
| expiration        | shortstr  | message expiration specification      |
+-------------------+-----------+---------------------------------------+
| message-id        | shortstr  | application message identifier        |
+-------------------+-----------+---------------------------------------+
| timestamp         | timestamp | message timestamp                     |
+-------------------+-----------+---------------------------------------+
| type              | shortstr  | message type name                     |
+-------------------+-----------+---------------------------------------+
| user-id           | shortstr  | creating user id                      |
+-------------------+-----------+---------------------------------------+
| app-id            | shortstr  | creating application id               |
+-------------------+-----------+---------------------------------------+
| reserved          | shortstr  | reserved, must be empty               |
+-------------------+-----------+---------------------------------------+

References
==========

- https://www.rabbitmq.com/tutorials/amqp-concepts.html
- https://www.rabbitmq.com/amqp-0-9-1-quickref.html
- https://www.amqp.org/specification/0-9-1/amqp-org-download
