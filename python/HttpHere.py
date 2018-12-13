import argparse
import errno
import socket
import SimpleHTTPServer
import SocketServer

httpd = None

try:
    parser = argparse.ArgumentParser(description='Serve a directory through HTTP.')
    parser.add_argument('--port', type=int, dest='port', action='store', default=8080, help='listening port, default is 8080')
    parser.add_argument('--correct', dest='correct', action='store_const', const=True, default=False, help='auto correct port if the specified one is not available')

    args = parser.parse_args()

    retry = False

    # try to serve at specified port
    # if cannot do it, retry
    try:
        httpd = SocketServer.TCPServer(("", args.port), SimpleHTTPServer.SimpleHTTPRequestHandler)
    except socket.error as e:
        if (e.errno == errno.EADDRINUSE) and (args.correct == True):
            retry = True
        else:
            raise

    if retry == True:
        httpd = SocketServer.TCPServer(("", 0), SimpleHTTPServer.SimpleHTTPRequestHandler)

    print 'Serving at', httpd.server_address
    httpd.serve_forever()

except KeyboardInterrupt:
    print 'Shutting down server.'

except socket.error as e:
    print e.strerror

except:
    print 'Unexpected exception, out.'

if httpd is not None:
    del httpd
