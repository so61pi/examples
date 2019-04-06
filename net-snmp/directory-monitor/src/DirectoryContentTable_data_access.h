/*
 * Note: this file originally auto-generated by mib2c using
 *       version $ of $
 *
 * $Id:$
 */
#ifndef DIRECTORYCONTENTTABLE_DATA_ACCESS_H
#define DIRECTORYCONTENTTABLE_DATA_ACCESS_H

#ifdef __cplusplus
extern "C" {
#endif


/* *********************************************************************
 * function declarations
 */

/* *********************************************************************
 * Table declarations
 */
/**********************************************************************
 **********************************************************************
 ***
 *** Table DirectoryContentTable
 ***
 **********************************************************************
 **********************************************************************/
/*
 * MONITOR-MIB::DirectoryContentTable is subid 4 of netSnmpModuleIDs.
 * Its status is Current.
 * OID: .1.3.6.1.4.1.8072.3.1.4, length: 10
*/


int DirectoryContentTable_init_data(
    DirectoryContentTable_registration* DirectoryContentTable_reg);


/*
 * TODO:180:o: Review DirectoryContentTable cache timeout.
 * The number of seconds before the cache times out
 */
#define DIRECTORYCONTENTTABLE_CACHE_TIMEOUT 60

void DirectoryContentTable_container_init(netsnmp_container** container_ptr_ptr,
                                          netsnmp_cache* cache);
void DirectoryContentTable_container_shutdown(netsnmp_container* container_ptr);

int DirectoryContentTable_container_load(netsnmp_container* container);
void DirectoryContentTable_container_free(netsnmp_container* container);

int DirectoryContentTable_cache_load(netsnmp_container* container);
void DirectoryContentTable_cache_free(netsnmp_container* container);

/*
***************************************************
***             START EXAMPLE CODE              ***
***---------------------------------------------***/
/* *********************************************************************
 * Since we have no idea how you really access your data, we'll go with
 * a worst case example: a flat text file.
 */
#define MAX_LINE_SIZE 256
/*
***---------------------------------------------***
***              END  EXAMPLE CODE              ***
***************************************************/
int DirectoryContentTable_row_prep(
    DirectoryContentTable_rowreq_ctx* rowreq_ctx);


#ifdef __cplusplus
}
#endif

#endif /* DIRECTORYCONTENTTABLE_DATA_ACCESS_H */
