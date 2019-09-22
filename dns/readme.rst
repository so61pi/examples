.. contents:: Table of Contents
.. section-numbering::

Zone File
=========

Format
------

From https://en.wikipedia.org/wiki/Zone_file

.. code-block:: text

    $ORIGIN example.com.     ; designates the start of this zone file in the namespace
    $TTL 1h                  ; default expiration time of all resource records without their own TTL value
    example.com.  IN  SOA   ns.example.com. username.example.com. ( 2007120710 1d 2h 4w 1h )
    example.com.  IN  NS    ns                    ; ns.example.com is a nameserver for example.com
    example.com.  IN  NS    ns.somewhere.example. ; ns.somewhere.example is a backup nameserver for example.com
    example.com.  IN  MX    10 mail.example.com.  ; mail.example.com is the mailserver for example.com
    @             IN  MX    20 mail2.example.com. ; equivalent to above line, "@" represents zone origin
    @             IN  MX    50 mail3              ; equivalent to above line, but using a relative host name
    example.com.  IN  A     192.0.2.1             ; IPv4 address for example.com
                  IN  AAAA  2001:db8:10::1        ; IPv6 address for example.com
    ns            IN  A     192.0.2.2             ; IPv4 address for ns.example.com
                  IN  AAAA  2001:db8:10::2        ; IPv6 address for ns.example.com
    www           IN  CNAME example.com.          ; www.example.com is an alias for example.com
    wwwtest       IN  CNAME www                   ; wwwtest.example.com is another alias for www.example.com
    mail          IN  A     192.0.2.3             ; IPv4 address for mail.example.com
    mail2         IN  A     192.0.2.4             ; IPv4 address for mail2.example.com
    mail3         IN  A     192.0.2.5             ; IPv4 address for mail3.example.com

Import Record Types
-------------------

- A/AAAA: Real IP address for a name.
- CNAME: Alias for a name.
- NS: Name of DNS server responsible for a domain/subdomain.

Iterative/Recursive Lookup
==========================

Glue Records
============

- Glue records are for breaking circular DNS dependencies.

      For example, if the authoritative name server for ``example.org`` is ``ns1.example.org``, a computer trying to resolve ``www.example.org`` first resolves ``ns1.example.org``. As ``ns1`` is contained in ``example.org``, this requires resolving ``example.org`` first, which presents a circular dependency. To break the dependency, the name server for the top level domain org includes glue along with the delegation for ``example.org``. The glue records are address records that provide IP addresses for ``ns1.example.org``. The resolver uses one or more of these IP addresses to query one of the domain's authoritative servers, which allows it to complete the DNS query.

      --- https://en.wikipedia.org/wiki/Domain_Name_System#Circular_dependencies_and_glue_records

Example
=======

``dig +trace +qr +multiline +all www.example.com``

#. Get root DNS servers. In the response we don't see the IP addresses of root servers but ``dig`` actually makes additional queries to get that information.

    Read question ``When I do a "dig . ns", many of the A records for the root servers are missing. Why?`` in https://www.bind9.net/BIND-FAQ

    .. code-block:: text

        ; <<>> DiG 9.11.2 <<>> +trace +qr +multiline +all www.example.com
        ;; global options: +cmd
        ;; Sending:
        ;; ->>HEADER<<- opcode: QUERY, status: NOERROR, id: 35934
        ;; flags: ad; QUERY: 1, ANSWER: 0, AUTHORITY: 0, ADDITIONAL: 1

        ;; OPT PSEUDOSECTION:
        ; EDNS: version: 0, flags: do; udp: 4096
        ; COOKIE: 001e369efc6b6e81
        ;; QUESTION SECTION:
        ;.                      IN NS

        ;; QUERY SIZE: 40

        ;; Got answer:
        ;; ->>HEADER<<- opcode: QUERY, status: NOERROR, id: 35934
        ;; flags: qr ra ad; QUERY: 1, ANSWER: 14, AUTHORITY: 0, ADDITIONAL: 1

        ;; OPT PSEUDOSECTION:
        ; EDNS: version: 0, flags: do; udp: 512
        ;; QUESTION SECTION:
        ;.                      IN NS

        ;; ANSWER SECTION:
        .                       212676 IN NS a.root-servers.net.
        .                       212676 IN NS b.root-servers.net.
        .                       212676 IN NS c.root-servers.net.
        .                       212676 IN NS d.root-servers.net.
        .                       212676 IN NS e.root-servers.net.
        .                       212676 IN NS f.root-servers.net.
        .                       212676 IN NS g.root-servers.net.
        .                       212676 IN NS h.root-servers.net.
        .                       212676 IN NS i.root-servers.net.
        .                       212676 IN NS j.root-servers.net.
        .                       212676 IN NS k.root-servers.net.
        .                       212676 IN NS l.root-servers.net.
        .                       212676 IN NS m.root-servers.net.
        .                       212676 IN RRSIG NS 8 0 518400 (
                                        20191004050000 20190921040000 59944 .
                                        Ov9DYrB0D8nkD1XgX6QU3qGMVAZR+3BCZpShL05wX+lv
                                        3GhNEN9G07vy7WmQVUidloecs+VhxCakMIVFwWoj0PP7
                                        7DO1Gq/1CYUBF/Pq8YkstpXKwGf6Rna2Mh7nDS+Qb3Ur
                                        f08Zf80jS0zQmMgo16niVxco1SKxjsE8x+klCCtlza0i
                                        PW88PRRD3PbMIMFgBWo/6HP6vqhm8DCUOK4xZ9pmNhEg
                                        GlH6e48Psx09zjijK3uYW1hNGRbmV0gLNGdIb+hsT1BS
                                        E9+aC1d0xEnmQmrlVD10sHco+/QeAUswxXm7X/LhR6Sg
                                        h98RnnD30TA2yta+OCXJHx48NiI0WXmH4w== )

        ;; Query time: 21 msec
        ;; SERVER: 8.8.8.8#53(8.8.8.8)
        ;; WHEN: Sun Sep 22 17:57:06 +07 2019
        ;; MSG SIZE  rcvd: 525

#. Choose one DNS server from the list and ask where ``www.example.com.`` is. The response contains a list of DNS servers (including a bunch of glue records) that are responsible for ``com.``.

    .. code-block:: text

        ;; Sending:
        ;; ->>HEADER<<- opcode: QUERY, status: NOERROR, id: 33339
        ;; flags: ad; QUERY: 1, ANSWER: 0, AUTHORITY: 0, ADDITIONAL: 1

        ;; OPT PSEUDOSECTION:
        ; EDNS: version: 0, flags: do; udp: 4096
        ; COOKIE: 001e369efc6b6e81
        ;; QUESTION SECTION:
        ;www.example.com.       IN A

        ;; QUERY SIZE: 56

        ;; Got answer:
        ;; ->>HEADER<<- opcode: QUERY, status: NOERROR, id: 33339
        ;; flags: qr; QUERY: 1, ANSWER: 0, AUTHORITY: 15, ADDITIONAL: 27

        ;; OPT PSEUDOSECTION:
        ; EDNS: version: 0, flags: do; udp: 4096
        ;; QUESTION SECTION:
        ;www.example.com.       IN A

        ;; AUTHORITY SECTION:
        com.                    172800 IN NS j.gtld-servers.net.
        com.                    172800 IN NS l.gtld-servers.net.
        com.                    172800 IN NS k.gtld-servers.net.
        com.                    172800 IN NS b.gtld-servers.net.
        com.                    172800 IN NS d.gtld-servers.net.
        com.                    172800 IN NS e.gtld-servers.net.
        com.                    172800 IN NS h.gtld-servers.net.
        com.                    172800 IN NS f.gtld-servers.net.
        com.                    172800 IN NS g.gtld-servers.net.
        com.                    172800 IN NS i.gtld-servers.net.
        com.                    172800 IN NS a.gtld-servers.net.
        com.                    172800 IN NS c.gtld-servers.net.
        com.                    172800 IN NS m.gtld-servers.net.
        com.                    86400 IN DS 30909 8 2 (
                                        E2D3C916F6DEEAC73294E8268FB5885044A833FC5459
                                        588F4A9184CFC41A5766 )
        com.                    86400 IN RRSIG DS 8 1 86400 (
                                        20191005050000 20190922040000 59944 .
                                        TY296qLivuT4OBQRtWHJj8Hm9rno2DyJkYFzOBsHrRWW
                                        VZH0n+a1OGTvC8+d7RNmp/EVqZgUCqlC6HPSq3wT/w0f
                                        L9Sqn18PEgKRw3VU7+GlOXaAb6j4QaLVj7qOKq9zQ0Dz
                                        /CS3IidAOa+EnQc2ae+hyBJEyje4cCHzaYJ1O0SVUh2Q
                                        7ypDbNKHJq35xv1qciA4JlBALaqHSZBn9Tw6cvD42el/
                                        qLfWBeKavJsiGqq4g6+l4OpE2LM26zepQ4uLJC6U82QP
                                        PzPCndAtUIcgUV5IF3l2Fh9vRjU8VNDJeW5vSdtCG3rT
                                        zoX6lhjLp+5jqBR3/UlobKKiUZQeAxtgEg== )

        ;; ADDITIONAL SECTION:
        m.gtld-servers.net.     172800 IN A 192.55.83.30
        l.gtld-servers.net.     172800 IN A 192.41.162.30
        k.gtld-servers.net.     172800 IN A 192.52.178.30
        j.gtld-servers.net.     172800 IN A 192.48.79.30
        i.gtld-servers.net.     172800 IN A 192.43.172.30
        h.gtld-servers.net.     172800 IN A 192.54.112.30
        g.gtld-servers.net.     172800 IN A 192.42.93.30
        f.gtld-servers.net.     172800 IN A 192.35.51.30
        e.gtld-servers.net.     172800 IN A 192.12.94.30
        d.gtld-servers.net.     172800 IN A 192.31.80.30
        c.gtld-servers.net.     172800 IN A 192.26.92.30
        b.gtld-servers.net.     172800 IN A 192.33.14.30
        a.gtld-servers.net.     172800 IN A 192.5.6.30
        m.gtld-servers.net.     172800 IN AAAA 2001:501:b1f9::30
        l.gtld-servers.net.     172800 IN AAAA 2001:500:d937::30
        k.gtld-servers.net.     172800 IN AAAA 2001:503:d2d::30
        j.gtld-servers.net.     172800 IN AAAA 2001:502:7094::30
        i.gtld-servers.net.     172800 IN AAAA 2001:503:39c1::30
        h.gtld-servers.net.     172800 IN AAAA 2001:502:8cc::30
        g.gtld-servers.net.     172800 IN AAAA 2001:503:eea3::30
        f.gtld-servers.net.     172800 IN AAAA 2001:503:d414::30
        e.gtld-servers.net.     172800 IN AAAA 2001:502:1ca1::30
        d.gtld-servers.net.     172800 IN AAAA 2001:500:856e::30
        c.gtld-servers.net.     172800 IN AAAA 2001:503:83eb::30
        b.gtld-servers.net.     172800 IN AAAA 2001:503:231d::2:30
        a.gtld-servers.net.     172800 IN AAAA 2001:503:a83e::2:30

        ;; Query time: 77 msec
        ;; SERVER: 193.0.14.129#53(193.0.14.129)
        ;; WHEN: Sun Sep 22 17:57:08 +07 2019
        ;; MSG SIZE  rcvd: 1178

#. Choose one DNS server from the list and ask where ``www.example.com.`` is. The response contains a list of DNS servers that are responsible for ``example.com.``. Notice that there are no glue records, do internally ``dig`` makes additional queries to get the IP addresses of newly received DNS servers.

    .. code-block:: text

        ;; Sending:
        ;; ->>HEADER<<- opcode: QUERY, status: NOERROR, id: 45212
        ;; flags: ad; QUERY: 1, ANSWER: 0, AUTHORITY: 0, ADDITIONAL: 1

        ;; OPT PSEUDOSECTION:
        ; EDNS: version: 0, flags: do; udp: 4096
        ; COOKIE: 001e369efc6b6e81
        ;; QUESTION SECTION:
        ;www.example.com.       IN A

        ;; QUERY SIZE: 56

        ;; Got answer:
        ;; ->>HEADER<<- opcode: QUERY, status: NOERROR, id: 45212
        ;; flags: qr; QUERY: 1, ANSWER: 0, AUTHORITY: 9, ADDITIONAL: 1

        ;; OPT PSEUDOSECTION:
        ; EDNS: version: 0, flags: do; udp: 4096
        ;; QUESTION SECTION:
        ;www.example.com.       IN A

        ;; AUTHORITY SECTION:
        example.com.            172800 IN NS a.iana-servers.net.
        example.com.            172800 IN NS b.iana-servers.net.
        example.com.            86400 IN DS 31589 8 1 (
                                        3490A6806D47F17A34C29E2CE80E8A999FFBE4BE )
        example.com.            86400 IN DS 31589 8 2 (
                                        CDE0D742D6998AA554A92D890F8184C698CFAC8A26FA
                                        59875A990C03E576343C )
        example.com.            86400 IN DS 43547 8 1 (
                                        B6225AB2CC613E0DCA7962BDC2342EA4F1B56083 )
        example.com.            86400 IN DS 43547 8 2 (
                                        615A64233543F66F44D68933625B17497C89A70E858E
                                        D76A2145997EDF96A918 )
        example.com.            86400 IN DS 31406 8 1 (
                                        189968811E6EBA862DD6C209F75623D8D9ED9142 )
        example.com.            86400 IN DS 31406 8 2 (
                                        F78CF3344F72137235098ECBBD08947C2C9001C7F6A0
                                        85A17F518B5D8F6B916D )
        example.com.            86400 IN RRSIG DS 8 2 86400 (
                                        20190927042220 20190920031220 17708 com.
                                        tj3ewj1Oay3W5u1JN9MOiD0Uw/mbzx9voe1I9rd4RQ2j
                                        T643Io5kWPy8wtNTIuaUv9kpEdeSa6uXYp14IzW3tI+n
                                        w9aV4ooRsrfvZgsnh9kisGXSWhX5Pg56fF1Om3cDrJqE
                                        OWAPYSwOx/Hee8PAedqlTimIhRcVvDRrW4IuDNs= )

        ;; Query time: 258 msec
        ;; SERVER: 192.41.162.30#53(192.41.162.30)
        ;; WHEN: Sun Sep 22 17:57:10 +07 2019
        ;; MSG SIZE  rcvd: 507

#. Choose one DNS server from the list and ask where ``www.example.com.`` is. The answer contains the IP address of ``www.example.com.``.

    .. code-block:: text

        ;; Sending:
        ;; ->>HEADER<<- opcode: QUERY, status: NOERROR, id: 60403
        ;; flags: ad; QUERY: 1, ANSWER: 0, AUTHORITY: 0, ADDITIONAL: 1

        ;; OPT PSEUDOSECTION:
        ; EDNS: version: 0, flags: do; udp: 4096
        ; COOKIE: 001e369efc6b6e81
        ;; QUESTION SECTION:
        ;www.example.com.       IN A

        ;; QUERY SIZE: 56

        ;; Got answer:
        ;; ->>HEADER<<- opcode: QUERY, status: NOERROR, id: 60403
        ;; flags: qr aa; QUERY: 1, ANSWER: 2, AUTHORITY: 3, ADDITIONAL: 1

        ;; OPT PSEUDOSECTION:
        ; EDNS: version: 0, flags: do; udp: 4096
        ;; QUESTION SECTION:
        ;www.example.com.       IN A

        ;; ANSWER SECTION:
        www.example.com.        86400 IN A 93.184.216.34
        www.example.com.        86400 IN RRSIG A 8 3 86400 (
                                        20191005230719 20190914222834 56575 example.com.
                                        g46QWewKdTmunpnqk7wHsewOzgQezuSoNwQRO8LD0W/7
                                        GC8OwV3HA1Zj5shCCArHxtXtfLCCYaGVZdLRaFW6Oc+g
                                        tHJazT7dhoGhTeB+eCtdSWlBkslW3cTWk8RxG0WWruEK
                                        a0Kjjjn/KldX9+XTxhLDQ7KDF0LPSV5KAH52ums= )

        ;; AUTHORITY SECTION:
        example.com.            86400 IN NS a.iana-servers.net.
        example.com.            86400 IN NS b.iana-servers.net.
        example.com.            86400 IN RRSIG NS 8 2 86400 (
                                        20191005153842 20190914202834 56575 example.com.
                                        ajxpO0fv2F5dsJyj2vd+qje5T7YGJJIEQJ170l/StG1G
                                        jtfDVUzyJu1nOD8YJI086LdAtlSP/TeAjOhfC/wG2HM3
                                        BHEoVMrXI608GULtxekWiyRzuGOAmAVSCReP8tOVyULq
                                        eaXzYuO3ngULe+EdeLjzz8jOLPkjpTnFXkdhJK8= )

        ;; Query time: 176 msec
        ;; SERVER: 199.43.133.53#53(199.43.133.53)
        ;; WHEN: Sun Sep 22 17:57:11 +07 2019
        ;; MSG SIZE  rcvd: 450

Additional Information
======================

- ``dig`` command has a hidden debug option, ``-d``. You can use it to get more insight about how ``dig`` works.
- To get the IP addresses of root DNS servers, run ``dig . NS @a.root-servers.net``.
- Some systems have a DNS cache. If your system happens to use ``nscd`` (you can check ``/var/run/nscd`` directory to see a socket file), then ``nscd -i hosts`` can help to clean its cache.
