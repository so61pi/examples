```
svr-main.c::main
    svr-runopts.c::svr_getopts (get command line options and save to svr_runopts.c::svr_opts)
    main_inetd (called when working in inetd mode (-i option))
        svr-session.c::svr_session
            common-channel.c::chaninitialise(svr_chantypes)
            ses.packettypes = svr_packettypes
            common-kex.c::send_msg_kexinit
            common-sesison.c::session_loop
                packet.c::write_packet
                packet.c::read_packet
                process-packet.c::process_packet
                    ses.packettypes[i].handler (handler is from svr_packettypes)
    main_noinetd (wait for new connection, accept it then fork)
        fork() after accepting new connection
        in child process : svr-session.c::svr_session


svr-session.c::svr_packettypes
    message                                 handler
    --------------------------------------------------------------------------------------------
    SSH_MSG_CHANNEL_DATA                    common-channel.c::recv_msg_channel_data
    SSH_MSG_CHANNEL_WINDOW_ADJUST           common-channel.c::recv_msg_channel_window_adjust
    SSH_MSG_USERAUTH_REQUEST                svr-auth.c::recv_msg_userauth_request
    SSH_MSG_SERVICE_REQUEST                 svr-service.c::recv_msg_service_request
    SSH_MSG_KEXINIT                         common-kex.c::recv_msg_kexinit
    SSH_MSG_KEXDH_INIT                      svr-kex.c::recv_msg_kexdh_init
    SSH_MSG_NEWKEYS                         common-kex.c::recv_msg_newkeys
    SSH_MSG_GLOBAL_REQUEST                  svr-tcpfwd.c::recv_msg_global_request_remotetcp
    SSH_MSG_CHANNEL_REQUEST                 common-channel.c::recv_msg_channel_request
    SSH_MSG_CHANNEL_OPEN                    common-channel.c::recv_msg_channel_open
    SSH_MSG_CHANNEL_EOF                     common-channel.c::recv_msg_channel_eof
    SSH_MSG_CHANNEL_CLOSE                   common-channel.c::recv_msg_channel_close
    SSH_MSG_CHANNEL_SUCCESS                 common-session.c::ignore_recv_response
    SSH_MSG_CHANNEL_FAILURE                 common-session.c::ignore_recv_response
    SSH_MSG_REQUEST_FAILURE                 common-session.c::ignore_recv_response
    SSH_MSG_REQUEST_SUCCESS                 common-session.c::ignore_recv_response
    SSH_MSG_CHANNEL_OPEN_CONFIRMATION       common-channel.c::recv_msg_channel_open_confirmation
    SSH_MSG_CHANNEL_OPEN_FAILURE            common-channel.c::recv_msg_channel_open_failure


common-channel.c::recv_msg_channel_request
    channel->type->reqhandler (can call chansessionrequest)


svr-chansession.c::svrchansess
    field name          value
    --------------------------------------
    sepfds              0
    name                "session"
    inithandler         newchansess
    checkclosehandler   sesscheckclose
    reqhandler          chansessionrequest
    closehandler        closechansess


svr-chansession.c::chansessionrequest
    sessioncommand (can add new subsystem here)


svr-tcpfwd.c::newtcpdirect (an example of how to do a tcp forwarding)
```
