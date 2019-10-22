.. contents:: Table of Contents
.. section-numbering::

Softwares
=========

- Linux

  +-----------------+---------------------------------------------------------------------------------+
  | Version         | v5.3                                                                            |
  +-----------------+---------------------------------------------------------------------------------+
  | Hash            | 4d856f72c10ecb060868ed10ff1b1453943fc6c8                                        |
  +-----------------+---------------------------------------------------------------------------------+
  | Source Location | https://github.com/torvalds/linux/tree/4d856f72c10ecb060868ed10ff1b1453943fc6c8 |
  +-----------------+---------------------------------------------------------------------------------+

Packet Arrival
==============

From Intel E1000

+ `e1000_receive_skb <https://github.com/torvalds/linux/blob/4d856f72c10ecb060868ed10ff1b1453943fc6c8/drivers/net/ethernet/intel/e1000e/netdev.c#L551>`__

  + ``napi_gro_receive``

    + ``napi_skb_finish``

      + `netif_receive_skb_internal <https://github.com/torvalds/linux/blob/4d856f72c10ecb060868ed10ff1b1453943fc6c8/net/core/dev.c#L5186>`__

From other places

+ `netif_receive_skb <https://github.com/torvalds/linux/blob/4d856f72c10ecb060868ed10ff1b1453943fc6c8/net/core/dev.c#L5261>`__

  + `netif_receive_skb_internal <https://github.com/torvalds/linux/blob/4d856f72c10ecb060868ed10ff1b1453943fc6c8/net/core/dev.c#L5186>`__

``netif_receive_skb_internal`` eventually calls into ``__netif_receive_skb_core``.

+ `netif_receive_skb_internal <https://github.com/torvalds/linux/blob/4d856f72c10ecb060868ed10ff1b1453943fc6c8/net/core/dev.c#L5186>`__

  + ``__netif_receive_skb``

    + ``__netif_receive_skb_one_core``

      + `__netif_receive_skb_core <https://github.com/torvalds/linux/blob/4d856f72c10ecb060868ed10ff1b1453943fc6c8/net/core/dev.c#L4807>`__

        + ``struct net_device::rx_handler``
        + ``deliver_ptype_list_skb``

          + ``deliver_skb``

            + ``struct packet_type::func``

In `__netif_receive_skb_core <https://github.com/torvalds/linux/blob/4d856f72c10ecb060868ed10ff1b1453943fc6c8/net/core/dev.c#L4807>`__ function

#. Call all ``struct packet_type::func`` handlers that are registered to ``ptype_all`` (global or device specific) except the last one.

   - This rarely happens.

#. Handle VLAN.

#. Call device's ``struct net_device::rx_handler``.

   - This path is usually taken by virtual devices like bridge or macvlan. 
   - ``struct net_device::rx_handler``

     * is set up by calling `netdev_rx_handler_register <https://github.com/torvalds/linux/blob/4d856f72c10ecb060868ed10ff1b1453943fc6c8/net/core/dev.c#L4728>`__.
     * examples are `br_handle_frame <https://github.com/torvalds/linux/blob/4d856f72c10ecb060868ed10ff1b1453943fc6c8/net/bridge/br_input.c#L256>`__ and `macvlan_handle_frame <https://github.com/torvalds/linux/blob/4d856f72c10ecb060868ed10ff1b1453943fc6c8/drivers/net/macvlan.c#L435>`__.
     * grep ``netdev_rx_handler_register`` to find more.

#. Call `deliver_ptype_list_skb <https://github.com/torvalds/linux/blob/4d856f72c10ecb060868ed10ff1b1453943fc6c8/net/core/dev.c#L1972>`__ which eventually calls ``struct packet_type::func``.

   - This is the most likely path for normal packets (no VLAN or exotic headers).
   - ``struct packet_type::func``

     * is set up by calling `dev_add_pack <https://github.com/torvalds/linux/blob/4d856f72c10ecb060868ed10ff1b1453943fc6c8/net/core/dev.c#L406>`__.
     * examples are `ip_rcv <https://github.com/torvalds/linux/blob/4d856f72c10ecb060868ed10ff1b1453943fc6c8/net/ipv4/ip_input.c#L514>`__ and `mpls_forward <https://github.com/torvalds/linux/blob/4d856f72c10ecb060868ed10ff1b1453943fc6c8/net/mpls/af_mpls.c#L341>`__.
     * grep ``dev_add_pack`` or ``static struct packet_type`` to find more.

IP Stack
========

Receiving
---------

+ `ip_rcv <https://github.com/torvalds/linux/blob/4d856f72c10ecb060868ed10ff1b1453943fc6c8/net/ipv4/ip_input.c#L514>`__ is injected to network stack `here <https://github.com/torvalds/linux/blob/4d856f72c10ecb060868ed10ff1b1453943fc6c8/net/ipv4/af_inet.c#L2023>`__.

  + ``ip_rcv_core``
  + ``NF_HOOK(NF_INET_PRE_ROUTING)``

    + ``ip_rcv_finish``

      + ``ip_rcv_finish_core``

        + ``ip_route_input_noref``

      + ``dst_input``

        + ``dst.input`` (= ``ip_local_deliver``)

+ ``ip_route_input_noref``

  + ``ip_route_input_rcu``

    + `ip_route_input_slow <https://github.com/torvalds/linux/blob/4d856f72c10ecb060868ed10ff1b1453943fc6c8/net/ipv4/route.c#L2034>`__

      + `make_route: <https://github.com/torvalds/linux/blob/4d856f72c10ecb060868ed10ff1b1453943fc6c8/net/ipv4/route.c#L2145>`__ path does forwarding

        + ``ip_mkroute_input``

          + ``__mkroute_input``

            + ``rt_dst_alloc``

              + ``dst.output`` = `ip_output <https://github.com/torvalds/linux/blob/4d856f72c10ecb060868ed10ff1b1453943fc6c8/net/ipv4/ip_output.c#L423>`__

            + ``dst.input`` = `ip_forward <https://github.com/torvalds/linux/blob/4d856f72c10ecb060868ed10ff1b1453943fc6c8/net/ipv4/ip_forward.c#L86>`__
            + ``skb_dst_set``

      + `local_input: <https://github.com/torvalds/linux/blob/4d856f72c10ecb060868ed10ff1b1453943fc6c8/net/ipv4/route.c#L2163>`__ path sends packets up to local processes

        + ``rt_dst_alloc``

          + ``dst.output`` = `ip_output <https://github.com/torvalds/linux/blob/4d856f72c10ecb060868ed10ff1b1453943fc6c8/net/ipv4/ip_output.c#L423>`__
          + ``dst.input`` = `ip_local_deliver <https://github.com/torvalds/linux/blob/4d856f72c10ecb060868ed10ff1b1453943fc6c8/net/ipv4/ip_input.c#L240>`__

        + ``skb_dst_set``

Send Packets To Local Processes
-------------------------------

+ `ip_local_deliver <https://github.com/torvalds/linux/blob/4d856f72c10ecb060868ed10ff1b1453943fc6c8/net/ipv4/ip_input.c#L240>`__

  + ``NF_HOOK(NF_INET_LOCAL_IN)``

    + ``ip_local_deliver_finish``

      + ``ip_protocol_deliver_rcu``

        + `tcp_v4_rcv <https://github.com/torvalds/linux/blob/4d856f72c10ecb060868ed10ff1b1453943fc6c8/net/ipv4/tcp_ipv4.c#L1786>`__ | ``udp_rcv``

``tcp_v4_rcv`` and ``udp_rcv`` are set up by `inet_add_protocol <https://github.com/torvalds/linux/blob/4d856f72c10ecb060868ed10ff1b1453943fc6c8/net/ipv4/protocol.c#L32>`__.

Forwarding
----------

+ `ip_forward <https://github.com/torvalds/linux/blob/4d856f72c10ecb060868ed10ff1b1453943fc6c8/net/ipv4/ip_forward.c#L86>`__

  + ``NF_HOOK(NF_INET_FORWARD)``

    + ``ip_forward_finish``

      + ``dst_output``

        + `ip_output <https://github.com/torvalds/linux/blob/4d856f72c10ecb060868ed10ff1b1453943fc6c8/net/ipv4/ip_output.c#L423>`__

Sending From Local Processes
----------------------------

+ ``ip_queue_xmit``

  + `__ip_queue_xmit <https://github.com/torvalds/linux/blob/4d856f72c10ecb060868ed10ff1b1453943fc6c8/net/ipv4/ip_output.c#L453>`__ does routing/sets up output

    + ``ip_local_out``

      + ``__ip_local_out``

        + ``nf_hook(NF_INET_LOCAL_OUT)``

          + ``dst_output``

            + ``dst.output`` (= `ip_output <https://github.com/torvalds/linux/blob/4d856f72c10ecb060868ed10ff1b1453943fc6c8/net/ipv4/ip_output.c#L423>`__)

Post Routing
------------

+ `ip_output <https://github.com/torvalds/linux/blob/4d856f72c10ecb060868ed10ff1b1453943fc6c8/net/ipv4/ip_output.c#L423>`__

  + ``NF_HOOK_COND(NF_INET_POST_ROUTING)``

    + ``ip_finish_output``

      + ``__ip_finish_output``

        + `ip_finish_output2 <https://github.com/torvalds/linux/blob/4d856f72c10ecb060868ed10ff1b1453943fc6c8/net/ipv4/ip_output.c#L185>`__

iptables
========

.. code-block:: text

                                           +-----------------+
                                           |                 |
                                         +>+  local process  +-+
                                         | |                 | |
                                         | +-----------------+ |
                                         |                     |
                                         |                     |
                                         |                     v
                                 +-------+------+      +-------+-------+
                                 |              |      |               |
                                 |    input     |      |    output     |
    mangle -> filter -> nat-src  |  (local in)  |      |  (local out)  |  raw -> mangle -> nat-dst -> filter
                                 |              |      |               |
                                 +-------+------+      +-------+-------+
                                         ^                     |
                                         |                     |
                                         |                     |
                    +---------------+    |    +-----------+    |    +----------------+
                    |               |    |    |           |    v    |                |
           +------->+  pre-routing  +----+--->+  forward  +----+--->+  post-routing  +------->
                    |               |         |           |         |                |
                    +---------------+         +-----------+         +----------------+

                raw -> mangle -> nat-dst     mangle -> filter        mangle -> nat-src


forward chain of filter table = rule-1 -> rule-2 -> rule-3 -> rule-4

  input chain of filter table = rule-1 -> rule-2    rule-3 -> rule-4
                                            |         ^
                                            V         |
           user-defined chain =           rule-a -> rule-b

- IPv4

  * Table

    + ``raw``

      - https://github.com/torvalds/linux/blob/4d856f72c10ecb060868ed10ff1b1453943fc6c8/net/ipv4/netfilter/iptable_raw.c#L21

    + ``mangle``

      - https://github.com/torvalds/linux/blob/4d856f72c10ecb060868ed10ff1b1453943fc6c8/net/ipv4/netfilter/iptable_mangle.c#L30

    + ``nat``

      - https://github.com/torvalds/linux/blob/4d856f72c10ecb060868ed10ff1b1453943fc6c8/net/ipv4/netfilter/iptable_nat.c#L18
      - https://github.com/torvalds/linux/blob/4d856f72c10ecb060868ed10ff1b1453943fc6c8/net/ipv4/netfilter/iptable_nat.c#L36

    + ``filter``

      - https://github.com/torvalds/linux/blob/4d856f72c10ecb060868ed10ff1b1453943fc6c8/net/ipv4/netfilter/iptable_filter.c

  * Priority

    + https://github.com/torvalds/linux/blob/4d856f72c10ecb060868ed10ff1b1453943fc6c8/include/uapi/linux/netfilter_ipv4.h#L30

  * `ipt_do_table <https://github.com/torvalds/linux/blob/4d856f72c10ecb060868ed10ff1b1453943fc6c8/net/ipv4/netfilter/ip_tables.c#L225>`__

  * `ipt_register_table <https://github.com/torvalds/linux/blob/4d856f72c10ecb060868ed10ff1b1453943fc6c8/net/ipv4/netfilter/ip_tables.c#L1755>`__

    * `nf_register_net_hooks <https://github.com/torvalds/linux/blob/4d856f72c10ecb060868ed10ff1b1453943fc6c8/net/netfilter/core.c#L473>`__

.. code-block: text

  a net namespace
      ipv4 netns_ipv4
          iptable_filter *xt_table
          iptable_mangle *xt_table
          iptable_raw *xt_table
          arptable_filter *xt_table
          nat_table *xt_table
      nf netns_nf
          hooks_ipv4 nf_hook_entries*[NF_INET_NUMHOOKS]
              num_hook_entries u16 
              hooks nf_hook_entry[]
                  hook nf_hookfn* // int(void *priv, struct sk_buff *skb, const struct nf_hook_state *state);
                  priv void*

  ipt_entry
      ip ipt_ip
          src          dst           in_addr
          smsk         dmsk          in_addr
          iniface      outiface      char[IFNAMSIZ]
          iniface_mask outiface_mask unsigned char[IFNAMSIZ]
      target_offset __u16        // Size of ipt_entry + matches
      next_offset   __u16        // Size of ipt_entry + matches + target
      comefrom      unsigned int // Back pointer
      elems                      // The matches (if any), then the target.

  struct xt_match
      // Return true or false: return FALSE and set *hotdrop = 1 to force immediate packet drop.
      bool (*match)(const struct sk_buff *skb, struct xt_action_param *);

  struct xt_target
      // Returns verdict. Modify skb.
      unsigned int (*target)(struct sk_buff *skb, const struct xt_action_param *);

An iptable is an array of ``ipt_entry`` elements with different sizes. We can jump from one element to the next one by using ``next_offset`` field. Inside ``ipt_do_table``, a ``jumpstack`` is used to save

Virtual Devices
===============

Bridge
------

.. code-block:: text

                         +--------+
               +---------+ bridge +----------+
               |         +--------+          |
               |                             |
    +----------+---------+        +----------+---------+
    | physical interface |        | physical interface |
    +--------------------+        +--------------------+

When adding a physical interface to a bridge, that device's ``rx_handler`` and ``rx_handler_data`` are set to ``br_handle_frame`` and ``struct net_bridge_port`` by calling ``netdev_rx_handler_register``.

+ `br_add_if <https://github.com/torvalds/linux/blob/4d856f72c10ecb060868ed10ff1b1453943fc6c8/net/bridge/br_if.c#L556>`__

  + `new_nbp <https://github.com/torvalds/linux/blob/4d856f72c10ecb060868ed10ff1b1453943fc6c8/net/bridge/br_if.c#L408>`__
  + `netdev_rx_handler_register <https://github.com/torvalds/linux/blob/4d856f72c10ecb060868ed10ff1b1453943fc6c8/net/core/dev.c#L4728>`__

When a packet enters the physical interface, it will go into `br_handle_frame <https://github.com/torvalds/linux/blob/4d856f72c10ecb060868ed10ff1b1453943fc6c8/net/bridge/br_input.c#L256>`__ function.

+ `br_handle_frame <https://github.com/torvalds/linux/blob/4d856f72c10ecb060868ed10ff1b1453943fc6c8/net/bridge/br_input.c#L256>`__

  + ``nf_hook_bridge_pre``

    + `br_handle_frame_finish <https://github.com/torvalds/linux/blob/4d856f72c10ecb060868ed10ff1b1453943fc6c8/net/bridge/br_input.c#L70>`__

      + working with forwarding database
      + `br_forward <https://github.com/torvalds/linux/blob/4d856f72c10ecb060868ed10ff1b1453943fc6c8/net/bridge/br_forward.c#L138>`__ | ``br_flood`` | ``br_multicast_flood``
      + `br_pass_frame_up <https://github.com/torvalds/linux/blob/4d856f72c10ecb060868ed10ff1b1453943fc6c8/net/bridge/br_input.c#L32>`__

        + ``NF_HOOK(NF_BR_LOCAL_IN)``

          + ``br_netif_receive_skb``

            + `netif_receive_skb <https://github.com/torvalds/linux/blob/4d856f72c10ecb060868ed10ff1b1453943fc6c8/net/core/dev.c#L5261>`__

Bonded Interface
----------------

Team Device
-----------

VLAN
----

VXLAN
-----

MACVLAN
-------

.. code-block:: text

    +--------------------------------------------+
    |                                            |
    |   +---------------+    +---------------+   |
    |   |    netns-1    |    |    netns-2    |   |
    |   |               |    |               |   |
    |   |   +-------+   |    |   +-------+   |   |
    |   |   | macv1 |   |    |   | macv2 |   |   |
    |   +---+---+---+---+    +---+---+---+---+   |
    |           |                    |           |
    |           +--------+  +--------+           |
    |                    |  |                    |
    |                  +-+--+-+                  |
    |                  | eth0 |                  |
    +------------------+------+------------------+
                          ||
                          ||
                          ||
                      +--------+
                      | switch |
                      +--------+

When adding an macvlan interface to a lower interface (e.g. ``eth0`` above), `macvlan_port_create <https://github.com/torvalds/linux/blob/4d856f72c10ecb060868ed10ff1b1453943fc6c8/drivers/net/macvlan.c#L1197>`__ change the lower interface's ``rx_handler`` and ``rx_handler_data`` to ``macvlan_handle_frame`` and ``struct macvlan_port``.

There are 5 modes for macvlan:

- Private

  * ``macv1`` and ``macv2`` have random MAC addresses.
  * They cannot communicate with each other.

- VEPA

  * ``macv1`` and ``macv2`` have random MAC addresses.
  * They can communicate with each other but ``switch`` must support hairpin mode.

- Bridge

  * ``macv1`` and ``macv2`` have random MAC addresses.
  * Packet switching between between ``macv1`` and ``macv2`` is done in ``eth0``.

- Passthru

  * ``macv1`` has same MAC address as ``eth0``.

- Source

  * Traffic is filtered based on source MAC addresses.

Some functions:

- `macvlan_handle_frame <https://github.com/torvalds/linux/blob/4d856f72c10ecb060868ed10ff1b1453943fc6c8/drivers/net/macvlan.c#L435>`__
- `macvlan_start_xmit <https://github.com/torvalds/linux/blob/4d856f72c10ecb060868ed10ff1b1453943fc6c8/drivers/net/macvlan.c#L548>`__
- `macvlan_newlink <https://github.com/torvalds/linux/blob/4d856f72c10ecb060868ed10ff1b1453943fc6c8/drivers/net/macvlan.c#L1506>`__

IPVLAN
------

MACVTAP/IPVTAP
--------------

Put TAP interfaces on top of MACVLAN/IPVLAN.

+ `macvtap_newlink <https://github.com/torvalds/linux/blob/4d856f72c10ecb060868ed10ff1b1453943fc6c8/drivers/net/macvtap.c#L81>`__

  + set up ``tap_handle_frame``
  + `macvlan_common_newlink <https://github.com/torvalds/linux/blob/4d856f72c10ecb060868ed10ff1b1453943fc6c8/drivers/net/macvlan.c#L1391>`__

+ `ipvtap_newlink <https://github.com/torvalds/linux/blob/4d856f72c10ecb060868ed10ff1b1453943fc6c8/drivers/net/ipvlan/ipvtap.c#L77>`__

  + set up ``tap_handle_frame``
  + `ipvlan_link_new <https://github.com/torvalds/linux/blob/4d856f72c10ecb060868ed10ff1b1453943fc6c8/drivers/net/ipvlan/ipvlan_main.c#L514>`__

+ `__netif_receive_skb_core <https://github.com/torvalds/linux/blob/4d856f72c10ecb060868ed10ff1b1453943fc6c8/net/core/dev.c#L4807>`__

  + ``rx_handler`` = `macvlan_handle_frame <https://github.com/torvalds/linux/blob/4d856f72c10ecb060868ed10ff1b1453943fc6c8/drivers/net/macvlan.c#L435>`__ | `ipvlan_handle_frame <https://github.com/torvalds/linux/blob/4d856f72c10ecb060868ed10ff1b1453943fc6c8/drivers/net/ipvlan/ipvlan_core.c#L729>`__

    + change ``skb->dev`` to ``vlan->dev`` which is ``tap->dev``

  + ``rx_handler`` = `tap_handle_frame <https://github.com/torvalds/linux/blob/4d856f72c10ecb060868ed10ff1b1453943fc6c8/drivers/net/tap.c#L318>`__

    + put packets into queue, wait for local process to get them.

MACsec
------

VETH
----

VETH interfaces come in pair, packets come in one end go out at other end.

+ `veth_newlink <https://github.com/torvalds/linux/blob/4d856f72c10ecb060868ed10ff1b1453943fc6c8/drivers/net/veth.c#L1236>`__

  + `create peer device <https://github.com/torvalds/linux/blob/4d856f72c10ecb060868ed10ff1b1453943fc6c8/drivers/net/veth.c#L1286>`__
  + `link two devices together <https://github.com/torvalds/linux/blob/4d856f72c10ecb060868ed10ff1b1453943fc6c8/drivers/net/veth.c#L1339-L1343>`__

+ `veth_xmit <https://github.com/torvalds/linux/blob/4d856f72c10ecb060868ed10ff1b1453943fc6c8/drivers/net/veth.c#L236>`__

  + `forward packet to peer <https://github.com/torvalds/linux/blob/4d856f72c10ecb060868ed10ff1b1453943fc6c8/drivers/net/veth.c#L262>`__

VCAN
----

VXCAN
-----

References
==========

- https://developers.redhat.com/blog/2018/10/22/introduction-to-linux-interfaces-for-virtual-networking/
