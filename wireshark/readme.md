**allow non-root users to run wireshark**

```shell
sudo setcap 'CAP_NET_RAW+eip CAP_NET_ADMIN+eip' `which dumpcap`
sudo setcap 'CAP_NET_RAW+eip CAP_NET_ADMIN+eip' `which tcpdump`
```
