#include "DirectoryStateNotification.h"

#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <errno.h>
#include <linux/limits.h>
#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/inotify.h>
#include <unistd.h>

#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>


extern const oid snmptrap_oid[];
extern const size_t snmptrap_oid_len;

static SNMPAlarmCallback send_notification;


typedef struct {
    int fd; /* file descriptor */
    int wd; /* watch descriptor*/
    char path[PATH_MAX + 1];
} monitor_data_t;


typedef struct {
    uint32_t mask; /* same as mask in inotify_event */
    char path[PATH_MAX + 1];
} event_info_t;


typedef struct {
    monitor_data_t monitor_info;
    unsigned int alarm;
} DirectoryStateNotificationData_t;


/*
 * format event_info_t into string with the following format
 *     EVENT_TYPE:ENTRY_TYPE:path
 */
static int event_info_to_string(event_info_t const* event_info, char* output,
                                size_t len) {
    assert(event_info && "event_info must not be NULL");

    char const* event_type = "UNKNOWN";
    if (event_info->mask & IN_CREATE) event_type = "CREATE";
    if (event_info->mask & IN_DELETE) event_type = "DELETE";
    if (event_info->mask & IN_MODIFY) event_type = "MODIFY";

    char const* entry_str = "UNKNOWN";
    if (event_info->mask & IN_ISDIR)
        entry_str = "DIRECTORY";
    else
        entry_str = "FILE";

    return snprintf(output, len, "%s:%s:%s", event_type, entry_str,
                    event_info->path);
}


/*
 * return 0 on error
 */
static int init_monitor(monitor_data_t* monitor_info) {
    assert(monitor_info && "directory must not be NULL");

    monitor_info->fd = inotify_init1(IN_NONBLOCK);
    if (monitor_info->fd == -1) return 0;

    monitor_info->wd = inotify_add_watch(monitor_info->fd, monitor_info->path,
                                         IN_CREATE | IN_DELETE | IN_MODIFY);
    if (monitor_info->wd == -1) return 0;

    return 1;
}


/*
 * clean up monitor
 */
static void uninit_monitor(monitor_data_t const* monitor_info) {
    assert(monitor_info && "monitor_info must not be NULL");

    inotify_rm_watch(monitor_info->fd, monitor_info->wd);
    close(monitor_info->fd);
}


/*
 * return number of events
 * if error, return -1, check errno
 * the caller must free event_infos
 */
static int handle_monitor_event(monitor_data_t const* monitor_info,
                                event_info_t** event_infos) {
    assert(monitor_info);
    assert(event_infos);

    char buffer[4096]
        __attribute__((aligned(__alignof__(struct inotify_event))));

    ssize_t len = read(monitor_info->fd, buffer, sizeof(buffer));
    if (len == -1 && errno != EAGAIN) return -1;
    if (len == 0) return 0;

    struct inotify_event const* event = NULL;
    char* ptr = buffer;
    int nevents = 0;

    /* get number of events */
    for (; ptr < buffer + len;
         ptr += sizeof(struct inotify_event) + event->len) {
        event = (struct inotify_event const*)ptr;
        ++nevents;
    }

    *event_infos = (event_info_t*)calloc(nevents, sizeof(event_info_t));
    if (*event_infos == NULL) return -1;

    /* loop through all events */
    int i = 0;
    for (ptr = buffer; ptr < buffer + len;
         ptr += sizeof(struct inotify_event) + event->len, ++i) {
        event = (struct inotify_event const*)ptr;
        (*event_infos + i)->mask = event->mask;
        strncpy((*event_infos + i)->path, event->name, event->len);
    }

    return nevents;
}


/*
 * *pdata will be NULL if error occurred
 */
void init_DirectoryStateNotification(void** pdata) {
    assert(pdata && "pdata must not be NULL");

    DEBUGMSGTL(("DirectoryStateNotification", "Initializing\n"));

    *pdata = NULL;
    DirectoryStateNotificationData_t* data =
        (DirectoryStateNotificationData_t*)malloc(
            sizeof(DirectoryStateNotificationData_t));
    if (data == NULL) return;

    strcpy(data->monitor_info.path, get_global_settings()->path);
    if (!init_monitor(&data->monitor_info)) {
        free(data);
        return;
    }

    data->alarm = snmp_alarm_register(1, SA_REPEAT, send_notification,
                                      &data->monitor_info);
    if (data->alarm == 0) {
        free(data);
        return;
    }

    *pdata = data;
    return;
}


void shutdown_DirectoryStateNotification(void* data) {
    assert(data && "data must not be NULL");

    DirectoryStateNotificationData_t* d =
        (DirectoryStateNotificationData_t*)data;
    uninit_monitor(&d->monitor_info);
    snmp_alarm_unregister(d->alarm);
    free(data);
}


/*
 * send single event info
 */
static void send_single_event_info(event_info_t const* event_info) {
    assert(event_info && "event_info must not be NULL");

    netsnmp_variable_list* var_list = NULL;
    const oid DirectoryStateNotification_oid[] = {1,    3, 6, 1, 4, 1,
                                                  8072, 7777, 2, 0, 1};
    const oid DirectoryState_oid[] = {1, 3, 6, 1, 4, 1, 8072, 7777, 2, 2, 1, 0};

    char buffer[1024] = {0};
    if (event_info_to_string(event_info, buffer, sizeof(buffer)) > 0) {
        /*
        * Set the snmpTrapOid.0 value
        */
        snmp_varlist_add_variable(&var_list, snmptrap_oid, snmptrap_oid_len,
                                  ASN_OBJECT_ID, DirectoryStateNotification_oid,
                                  sizeof(DirectoryStateNotification_oid));


        /*
        * Add any objects from the trap definition
        */
        snmp_varlist_add_variable(&var_list, DirectoryState_oid,
                                  OID_LENGTH(DirectoryState_oid), ASN_OCTET_STR,
                                  buffer, strlen(buffer));
        send_v2trap(var_list);
        snmp_free_varbind(var_list);
    }
}


/*
 * send event info notifications
 */
static void send_notification(unsigned int clientreg, void* clientarg) {
    (void)clientreg;

    assert(clientarg && "clientarg must not be NULL");

    monitor_data_t const* monitor_info = (monitor_data_t const*)clientarg;
    event_info_t* event_info = NULL;
    int numEvents = handle_monitor_event(monitor_info, &event_info);
    int i = 0;
    for (; i < numEvents; ++i) {
        send_single_event_info(event_info + i);
    }
    free(event_info);
}
