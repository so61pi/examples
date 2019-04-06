#include "NumFiles.h"

#include <assert.h>
#include <stdlib.h>

#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>

#include "Utility.h"


static Netsnmp_Node_Handler handle_NumFiles;


typedef struct { char const* path; } NumFilesData_t;


static NumFilesData_t* g_data;


/** Initializes the NumFiles module */
void init_NumFiles(void** pdata) {
    assert(pdata && "pdata must not be NULL");

    const oid NumFiles_oid[] = {1, 3, 6, 1, 4, 1, 8072, 7777, 1, 2};

    DEBUGMSGTL(("NumFiles", "Initializing\n"));

    *pdata = NULL;
    NumFilesData_t* data = (NumFilesData_t*)malloc(sizeof(NumFilesData_t));
    if (data == NULL) return;

    data->path = get_global_settings()->path;
    netsnmp_register_scalar(
        netsnmp_create_handler_registration("NumFiles", handle_NumFiles,
                                            NumFiles_oid,
                                            OID_LENGTH(NumFiles_oid),
                                            HANDLER_CAN_RONLY));
    g_data = data;
    *pdata = data;
}


void shutdown_NumFiles(void* data) {
    assert(data && "data must not be NULL");

    free(data);
}


static int handle_NumFiles(netsnmp_mib_handler* handler,
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
        int nfiles = get_num_of_files(g_data->path);
        snmp_set_var_typed_value(requests->requestvb, ASN_INTEGER, &nfiles,
                                 sizeof(nfiles));
        break;
    }


    default:
        /* we should never get here, so this is a really bad error */
        snmp_log(LOG_ERR, "unknown mode (%d) in handle_NumFiles\n",
                 reqinfo->mode);
        return SNMP_ERR_GENERR;
    }

    return SNMP_ERR_NOERROR;
}
