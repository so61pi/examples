#include "Directory.h"

#include <assert.h>
#include <stdlib.h>

#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>


static Netsnmp_Node_Handler handle_Directory;


typedef struct { char const* path; } DirectoryData_t;


static DirectoryData_t* g_data;


/** Initializes the Directory module */
void init_Directory(void** pdata) {
    assert(pdata && "pdata must not be NULL");

    const oid Directory_oid[] = {1, 3, 6, 1, 4, 1, 8072, 7777, 1, 1};

    DEBUGMSGTL(("Directory", "Initializing\n"));

    *pdata = NULL;
    DirectoryData_t* data = (DirectoryData_t*)malloc(sizeof(DirectoryData_t));
    if (data == NULL) return;

    data->path = get_global_settings()->path;
    netsnmp_register_scalar(
        netsnmp_create_handler_registration("Directory", handle_Directory,
                                            Directory_oid,
                                            OID_LENGTH(Directory_oid),
                                            HANDLER_CAN_RWRITE));
    g_data = data;
    *pdata = data;
}


void shutdown_Directory(void* data) {
    assert(data && "data must not be NULL");

    free(data);
}


static int handle_Directory(netsnmp_mib_handler* handler,
                            netsnmp_handler_registration* reginfo,
                            netsnmp_agent_request_info* reqinfo,
                            netsnmp_request_info* requests) {
    (void)handler;
    (void)reginfo;

    int ret;
    /* We are never called for a GETNEXT if it's registered as a
     *      "instance", as it's "magically" handled for us.  */

    /* a instance handler also only hands us one request at a time, so
     *      we don't need to loop over a list of requests; we'll only get one.
     */

    switch (reqinfo->mode) {

    case MODE_GET:
        snmp_set_var_typed_value(requests->requestvb, ASN_OCTET_STR,
                                 g_data->path, strlen(g_data->path));
        break;

    /*
     * SET REQUEST
     *
     * multiple states in the transaction.  See:
     * http://www.net-snmp.org/tutorial-5/toolkit/mib_module/set-actions.jpg
     */
    case MODE_SET_RESERVE1:
        /* or you could use netsnmp_check_vb_type_and_size instead */
        ret = netsnmp_check_vb_type(requests->requestvb, ASN_OCTET_STR);
        if (ret != SNMP_ERR_NOERROR) {
            netsnmp_set_request_error(reqinfo, requests, ret);
        }
        break;

    case MODE_SET_RESERVE2:
        /* XXX malloc "undo" storage buffer */
        /*
        if (XXX if malloc, or whatever, failed:) {
            netsnmp_set_request_error(reqinfo, requests,
        SNMP_ERR_RESOURCEUNAVAILABLE);
        }
        */
        break;

    case MODE_SET_FREE:
        /* XXX: free resources allocated in RESERVE1 and/or
         *              RESERVE2.  Something failed somewhere, and the states
         *              below won't be called. */
        break;

    case MODE_SET_ACTION:
        /* XXX: perform the value change here */
        /*
        if (XXX: error?) {
            netsnmp_set_request_error(reqinfo, requests, some error);
        }
        */
        strncpy(get_mutable_global_settings()->path,
                (char const*)requests->requestvb->val.string,
                sizeof(get_mutable_global_settings()->path));
        break;

    case MODE_SET_COMMIT:
        /* XXX: delete temporary storage */
        /*
        if (XXX: error?) {
            netsnmp_set_request_error(reqinfo, requests, SNMP_ERR_COMMITFAILED);
        }
        */
        break;

    case MODE_SET_UNDO:
        /* XXX: UNDO and return to previous value for the object */
        /*
        if (XXX: error?) {
            netsnmp_set_request_error(reqinfo, requests, SNMP_ERR_UNDOFAILED);
        }
        */
        break;

    default:
        /* we should never get here, so this is a really bad error */
        snmp_log(LOG_ERR, "unknown mode (%d) in handle_Directory\n",
                 reqinfo->mode);
        return SNMP_ERR_GENERR;
    }

    return SNMP_ERR_NOERROR;
}
