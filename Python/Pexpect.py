#! /usr/bin/python3

import sys
import pexpect

if __name__ == '__main__':
    info = {'host' : '127.0.0.1', 'port' : '2222', 'user' : 'vagrant', 'pass' : 'vagrant', 'prompt' : 'vagrant@'}

    child = pexpect.spawn('ssh -o StrictHostKeyChecking=no -o UserKnownHostsFile=/dev/null -p {port} {user}@{host}'.format(port=info['port'], user=info['user'], host=info['host']))
    child.delaybeforesend = None

    i = child.expect([pexpect.TIMEOUT, 'password: '])
    if i == 0:
        print('[error] timeout')
        sys.exit(1)
    else:
        child.sendline(info['pass'])

    child.expect(info['prompt'])
    print(child.before)
    
    child.sendline('ls -alh')
    child.expect(info['prompt'])
    print(child.before)

    child.sendline('exit')
