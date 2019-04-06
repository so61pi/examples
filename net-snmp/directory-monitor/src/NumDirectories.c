#include "NumDirectories.h"

#include <assert.h>
#include <stdlib.h>

#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>

#include "Utility.h"


static Netsnmp_Node_Handler handle_NumDirectories;


typedef struct { char const* path; } NumDirectoriesData_t;


static NumDirectoriesData_t* g_data;


/** Initializes the NumDirectories module */
void init_NumDirectories(void** pdata) {
    assert(pdata && "pdata must not be NULL");

    const oid NumDirectories_oid[] = {1, 3, 6, 1, 4, 1, 8072, 7777, 1, 3};

    DEBUGMSGTL(("NumDirectories", "Initializing\n"));

    *pdata = NULL;
    NumDirectoriesData_t* data =
        (NumDirectoriesData_t*)malloc(sizeof(NumDirectoriesData_t));
    if (data == NULL) return;

    data->path = get_global_settings()->path;
    netsnmp_register_scalar(
        netsnmp_create_handler_registration("NumDirectories",
                                            handle_NumDirectories,
                                            NumDirectories_oid,
                                            OID_LENGTH(NumDirectories_oid),
                                            HANDLER_CAN_RONLY));
    g_data = data;
    *pdata = data;
}


void shutdown_NumDirectories(void* data) {
    assert(data && "data must not be NULL");

    free(data);
}


static int handle_NumDirectories(netsnmp_mib_handler* handler,
                                 netsnmp_handler_registration* reginfo,
                                 netsnmp_agent_request_info* reqinfo,
                                 netsnmp_request_info* requests) {
    (void)handler;
    (void)reginfo;

    /* We are never called for a GETNEXT if it's registered as a
       "instance", as it's "magically" handled for us.  */

    /* a instance handler also only hands us one request at a time, so
       we don't need to loop over a list of requests; we'll only get one. */

    switch (reqinfo->mode) {
    case MODE_GET: {
        int ndirectories = get_num_of_directories(g_data->path);
        snmp_set_var_typed_value(requests->requestvb, ASN_INTEGER,
                                 &ndirectories, sizeof(ndirectories));
        break;
    }


    default:
        /* we should never get here, so this is a really bad error */
        snmp_log(LOG_ERR, "unknown mode (%d) in handle_NumDirectories\n",
                 reqinfo->mode);
        return SNMP_ERR_GENERR;
    }

    return SNMP_ERR_NOERROR;
}
