A simple NetSNMP agent (subagent) monitoring a directory.

Its features are:
    - Can change monitored directory.
    - Get number of files/sub directories in current directory.
    - Get content of current directory (file name, permission).
    - Report file or directory changes (CREATE DELETE MODIFY, through notifications).

It can run as a master agent or a sub agent.
You can read its usage by option -h.

usage: moninor-agent -a -p [-h] [-d] [-D<tokens>] [-f] [-L] [-M] [-H] [LISTENING ADDRESSES]
    -a  Set configuration file name.
        Example: -a snmpd -> configuration file snmpd.conf
    -p  Set working directory.
        Path must be end with '\'.
    -h  Print this help.
    -f  Do not fork() from the calling shell.
    -d  Print all available TOKENs.
    -DTOKEN[,TOKEN,...]
        Turn on debugging output for the given TOKEN(s).
        Without any tokens specified, it defaults to printing
        all the tokens (which is equivalent to the keyword 'ALL').
        You might want to try ALL for extremely verbose output.
        Note: You can't put a space between the -D and the TOKENs.
    -M  Run as a normal SNMP Agent instead of an AgentX sub-agent.
    -x ADDRESS    connect to master agent at ADDRESS (default /var/agentx/master).
    -L    Do not open a log file; print all messages to stderr.


Example run command:
    sudo ./monitor-agent.out -a snmpd -p /home/ -f -L -M

Note: To use this program you need net-snmp, this can be done by compiling from source or installing with this command:
    apt-get install snmp snmp-mibs-downloader libsnmp-perl libsnmp-dev
