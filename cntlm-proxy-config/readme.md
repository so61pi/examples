## Introduction

In some cases you cannot use normal proxy, then cntlm may help. It creates an additional proxy between your program and the parent proxy server (your corporate's proxy server).

Read more here:

- http://cntlm.sourceforge.net
- http://linux.die.net/man/1/cntlm

## Install cntlm

```bash
sudo apt-get install cntlm
```

## Change cntlm's config file

- It is at `/etc/cntlm.conf`
- First, add your parent proxy address:

```txt
# Your parent proxy's username
# This is unnecessary if you don't have it
Username        testuser

# Parent proxy server's address
Proxy           10.10.10.10:8080

# cntlm proxy
# This is the proxy address for other programs
Listen          127.0.0.1:3128
```

- Then, if you have a password for parent proxy, generate hash for it:

```shell
cntlm -H
Password: <type your password here>
PassLM          5F9D1B94DB690CDF552C4BCA4AEBFB11
PassNT          4E2712FDBEA19C26190A65560A5DE6C6
PassNTLMv2      080CBBE03E0A4074CB6FEA544767B668
```

- Copy these lines to `/etc/cntlm.conf`

## Reload cntlm service

```shell
sudo service cntlm reload
```

## Use cntlm proxy

```shell
export http_proxy=http://127.0.0.1:3128/
```
