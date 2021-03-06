/*
 * Note: this file originally auto-generated by mib2c using
 *       version $ of $
 */
/* standard Net-SNMP includes */
#include <errno.h>
#include <dirent.h>
#include <signal.h>

#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-features.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>

/* include our parent header */
#include "Directory.h"
#include "DirectoryContentTable.h"
#include "DirectoryStateNotification.h"
#include "GlobalSettings.h"
#include "NumDirectories.h"
#include "NumFiles.h"


/*
 * If compiling within the net-snmp source code, this will trigger the feature
 * detection mechansim to ensure the agent_check_and_process() function
 * is left available even if --enable-minimialist is turned on.  If you
 * have configured net-snmp using --enable-minimialist and want to compile
 * this code externally to the Net-SNMP code base, then please add
 * --with-features="agent_check_and_process enable_stderrlog" to your
 * configure line.
 */
netsnmp_feature_require(agent_check_and_process)
netsnmp_feature_require(enable_stderrlog)


static int g_keep_running = 1;


static RETSIGTYPE stop_server(int a) { g_keep_running = 0; }


static void print_usage(void) {
    printf(
        "usage: moninor-agent -a -p [-h] [-d] [-D<tokens>] [-f] [-L] [-M] [-H] [LISTENING ADDRESSES]\n"
        "\t-a\tSet configuration file name.\n"
        "\t\tExample: -a snmpd -> configuration file snmpd.conf\n"

        "\t-p\tSet working directory.\n"
        "\t\tPath must be end with '\\'.\n"

        "\t-h\tPrint this help.\n"

        "\t-f\tDo not fork() from the calling shell.\n"

        "\t-d\tPrint all available TOKENs.\n"

        "\t-DTOKEN[,TOKEN,...]\n"
        "\t\tTurn on debugging output for the given TOKEN(s).\n"
        "\t\tWithout any tokens specified, it defaults to printing\n"
        "\t\tall the tokens (which is equivalent to the keyword 'ALL').\n"
        "\t\tYou might want to try ALL for extremely verbose output.\n"
        "\t\tNote: You can't put a space between the -D and the TOKENs.\n"

        "\t-M\tRun as a normal SNMP Agent instead of an AgentX sub-agent.\n"

        "\t-x ADDRESS\tconnect to master agent at ADDRESS (default "
        "/var/agentx/master).\n"

        "\t-L\tDo not open a log file; print all messages to stderr.\n");

    exit(0);
}


static void print_tokens() {
    printf(
        "Available tokens:\n"
        "\tDirectory\n"
        "\tDirectoryStateNotification\n"
        "\tNumDirectories\n"
        "\tNumFiles\n"
        "\tDirectoryContentTable\n");
}


int main(int argc, char** argv) {
    int agentx_subagent =
        1; /* change this if you want to be a SNMP master agent */
    /* Defs for arg-handling code: handles setting of policy-related variables
     */
    int ch;
    extern char* optarg;
    int dont_fork = 0, use_syslog = 0;
    char const* agentx_socket = NULL;
    char const* app_name = "moninor-agent";
    char const* directory = "/home/";

    while ((ch = getopt(argc, argv, "a:p:hdD:fHLMx:")) != EOF) {
        switch (ch) {
        case 'a':
            app_name = optarg;
            break;
        case 'p':
            directory = optarg;
            break;
        case 'h':
            print_usage();
            exit(0);
        case 'd':
            print_tokens();
            exit(0);
            break;
        case 'D':
            debug_register_tokens(optarg);
            snmp_set_do_debugging(1);
            break;
        case 'f':
            dont_fork = 1;
            break;
        case 'M':
            agentx_subagent = 0;
            break;
        case 'L':
            use_syslog = 0; /* use stderr */
            break;
        case 'x':
            agentx_socket = optarg;
            break;
        default:
            fprintf(stderr, "unknown option %c\n", ch);
            print_usage();
        }
    }

    if (optind < argc) {
        int i;
        /*
            * There are optional transport addresses on the command line.
            */
        DEBUGMSGTL(("snmpd/main", "optind %d, argc %d\n", optind, argc));
        for (i = optind; i < argc; i++) {
            char* c, *astring;
            if ((c = netsnmp_ds_get_string(NETSNMP_DS_APPLICATION_ID,
                                           NETSNMP_DS_AGENT_PORTS))) {
                astring = malloc(strlen(c) + 2 + strlen(argv[i]));
                if (astring == NULL) {
                    fprintf(stderr, "malloc failure processing argv[%d]\n", i);
                    exit(1);
                }
                sprintf(astring, "%s,%s", c, argv[i]);
                netsnmp_ds_set_string(NETSNMP_DS_APPLICATION_ID,
                                      NETSNMP_DS_AGENT_PORTS, astring);
                SNMP_FREE(astring);
            } else {
                netsnmp_ds_set_string(NETSNMP_DS_APPLICATION_ID,
                                      NETSNMP_DS_AGENT_PORTS, argv[i]);
            }
        }
        DEBUGMSGTL(("snmpd/main", "port spec: %s\n",
                    netsnmp_ds_get_string(NETSNMP_DS_APPLICATION_ID,
                                          NETSNMP_DS_AGENT_PORTS)));
    }

    /* check directory existence */
    DIR* dir = opendir(directory);
    if (dir) {
        closedir(dir);
    } else if (ENOENT == errno) {
        puts("Directory doesn't not exist.");
        exit(-1);
    } else {
        puts("Cannot open direcory.");
        exit(-1);
    }

    /* we're an agentx subagent? */
    if (agentx_subagent) {
        /* make us a agentx client. */
        netsnmp_enable_subagent();
        if (NULL != agentx_socket) {
            netsnmp_ds_set_string(NETSNMP_DS_APPLICATION_ID,
                                  NETSNMP_DS_AGENT_X_SOCKET, agentx_socket);
        }
    }

    snmp_disable_log();
    if (use_syslog)
        snmp_enable_calllog();
    else
        snmp_enable_stderrlog();

    /* daemonize */
    if (!dont_fork) {
        int rc = netsnmp_daemonize(1, !use_syslog);
        if (rc) exit(-1);
    }

    /* initialize tcp/ip if necessary */
    SOCK_STARTUP;

    /* initialize the agent library */
    init_agent(app_name);

    /* init mib code */
    global_settings_t* settings = get_mutable_global_settings();
    strcpy(settings->path, directory);

    void* data[4] = {NULL, NULL, NULL, NULL};
    init_Directory(data + 0);
    init_DirectoryStateNotification(data + 1);
    init_NumDirectories(data + 2);
    init_NumFiles(data + 3);
    init_DirectoryContentTable();

    /* read app_name.conf files. */
    init_snmp(app_name);

    /* If we're going to be a snmp master agent, initial the ports */
    if (!agentx_subagent)
        init_master_agent(); /* open the port to listen on (defaults to udp:161)
                                */

    /* In case we recevie a request to stop (kill -TERM or kill -INT) */
    g_keep_running = 1;
    signal(SIGTERM, stop_server);
    signal(SIGINT, stop_server);

    /* you're main loop here... */
    while (g_keep_running) {
        agent_check_and_process(1); /* 0 == don't block */
    }

    shutdown_DirectoryContentTable();
    shutdown_NumFiles(data[3]);
    shutdown_NumDirectories(data[2]);
    shutdown_DirectoryStateNotification(data[1]);
    shutdown_Directory(data[0]);

    /* at shutdown time */
    snmp_shutdown(app_name);
    SOCK_CLEANUP;
    exit(0);
}
