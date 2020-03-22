.. contents:: Table of Contents

Tools
=====

- netdata

  This tool is used to monitor system information. It has its own UI interface available at ``http://localhost:19999``, but it also exports metrics to other tools.

- node-exporter

  Similar to ``netdata`` but only for Prometheus.

- ebpf-exporter

  This tool supports monitoring Linux kernel through eBPF and exports metrics to Prometheus.

- blackbox-exporter

  This utility is used to monitor external hosts. We can use it to check if a web site is still accessible using HTTP, ICMP requests.

  As we do not use this tool to monitor the system it is in, we can install it inside a container.

Installation
============

- On host system

  * netdata
  * ebpf-exporter
  * node-exporter

  They must also be configured to start at boot time too. ``netdata`` also does that during its installation process.

  For the other two, we create systemd service files and place them in ``/etc/systemd/system`` directory, then run ``sudo systemctl enable xxx`` to make them run at boot.

  The configurations for these services can be found in current directory.

- In container

  * prometheus
  * grafana
  * blackbox-exporter

  There is a ``docker-compose.yml`` file for these three tools in this directory. We can start them by ``docker-compose up -d``.

Creating Dashboard
==================

After installing all of the prerequisites, we can now create dashboards for available metrics by going to grafana UI at ``http://localhost:3000``.

There are four dashboards in ``dashboards`` directory ready for importing.
