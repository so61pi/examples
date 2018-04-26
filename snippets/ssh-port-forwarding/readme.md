## Local port forwarding

```txt
# on localhost
ssh -N -f -L 9000:example.com:80 admin@server.com

localhost:9000 <--------------------> server <--------------------> example.com:80
                                        /\
                        ________________||_________________
                       |                                   |
                       |  All packets from localhost:9000  |
                       | are transferred to example.com:80 |
                       |___________________________________|


ssh -N -f -L *:9000:example.com:80 admin@server.com

*:9000 <--------------------> server <--------------------> example.com:80
                                /\
                ________________||_________________
               |                                   |
               |      All packets from *:9000      |
               | are transferred to example.com:80 |
               |___________________________________|
```

## Remote port forwarding

```txt
# on localhost
ssh -N -f -R 9000:localhost:3000 admin@server.com

localhost:3000 <--------------------> server:9000 <--------------------> internet
                                           /\
                       ____________________||____________________
                      |                                          |
                      | All packets from internet to server:9000 |
                      |    are transferred to localhost:3000     |
                      |__________________________________________|


ssh -N -f -R 9000:address:3000 admin@server.com

address:3000 <--------------------> server:9000 <--------------------> internet
                                         /\
                     ____________________||____________________
                    |                                          |
                    | All packets from internet to server:9000 |
                    |     are transferred to address:3000      |
                    |__________________________________________|

#
# on server
#
sudo vim /etc/ssh/sshd_config
# add GatewayPorts yes

sudo service ssh restart
#sudo systemctl restart ssh
```

## SOCKS proxy

```txt
ssh -N -f -D localhost:3000 admin@server.com

               --------------------
localhost:3000 ==================== server.com
               --------------------
# use localhost:3000 as a SOCKS proxy address
```

## Enable keep-alive

- Add below line to `~/.ssh/config`

```txt
ServerAliveInterval 180
```
