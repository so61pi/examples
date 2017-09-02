**allow non-root users to run wireshark**

```shell
# sudo dpkg-reconfigure wireshark-common
# -> choose YES
sudo usermod -aG wireshark $USER
```
