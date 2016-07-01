#! /usr/bin/env python

from scapy.all import *


eth   = Ether(src = '00:00:00:00:00:11', dst = 'ff:ff:ff:ff:ff:ff')
ip    = IPv6(src = 'fe80::cc9f:94b5:48d7:8b6b', dst = 'ff02::1:ff84:6a85')
ping  = ICMPv6EchoRequest()
ns    = ICMPv6ND_NS(tgt = 'fe80::586b:866a:3084:6a85')
nsOpt = ICMPv6NDOptSrcLLAddr(lladdr = '00:00:00:00:00:11')

# send IPv6 Echo Request
pkt = eth / ip / ping
sendp(pkt, iface = 'ens3')
pkt.show2()

# send 1024 IPv6 Network Discovery Neighbor Solicitation packets
for i in range(10, 10 + 1024):
    ee = (i & 0xFF00) >> 8
    ff = (i & 0xFF)
    srcMac = '00:00:00:00:%02x:%02x' % (ee, ff)

    eth.src      = srcMac
    ip.src       = 'fe80::cc9f:94b5:48d7:%04x' % i
    nsOpt.lladdr = srcMac

    pkt = eth / ip / ns / nsOpt
    sendp(pkt, iface = 'ens3')
    pkt.show2()

