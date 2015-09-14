#include <stdio.h>  
#include <string.h>  
#include <stdlib.h>  
#include <stdbool.h>
#include <stdint.h>
#include <inttypes.h>

 


#define MAXNAMELEN 100
#define MAXIPLEN 16
#define MAXCHAPPWD 16
#define MAXCHAPUSER 100
#define MAXLUNS 10
#define MAXACLS 100
#define MAXPORTALS 21
#define MAXWWNLEN 100


typedef struct storage_obj
{
    char dev[MAXNAMELEN];  // "/home/admin/sparcedata",
    char name[MAXNAMELEN]; // "vol4",
    int  plugin;            // "fileio", "blockio"
    uint64_t size;          // 2147483648,
    int write_back;         // true,
    char wwn[MAXNAMELEN];    // "dffc64ce-1e34-4410-9531-b71bb7b44848"
} storage_obj_t, *sobj_t;


typedef struct portals
{    
    char ip_address[MAXIPLEN];
    int iser;   // true or false
    int port;
} portals_t,*port_t;



typedef struct  parameters
{
    int AuthMethod;   // "CHAP = 1,None = 2", "CHAP mutual = 3", etc
#if 0
        "DataDigest": "CRC32C,None",
        "DataPDUInOrder": "Yes",
        "DataSequenceInOrder": "Yes",
        "DefaultTime2Retain": "20",
        "DefaultTime2Wait": "2",
        "ErrorRecoveryLevel": "0",
        "FirstBurstLength": "65536",
        "HeaderDigest": "CRC32C,None",
        "IFMarkInt": "2048~65535",
        "IFMarker": "No",
        "ImmediateData": "Yes",
        "InitialR2T": "Yes",
        "MaxBurstLength": "262144",
        "MaxConnections": "1",
        "MaxOutstandingR2T": "1",
        "MaxRecvDataSegmentLength": "8192",
        "MaxXmitDataSegmentLength": "262144",
        "OFMarkInt": "2048~65535",
        "OFMarker": "No",
#endif
    char TargetAlias[MAXNAMELEN];  //"LIO Target"
} parameters_t, *para_t;



typedef struct attributes
{
    bool authentication;
    bool cache_dynamic_acls;
    int default_cmdsn_depth;
    bool demo_mode_write_protect;
    bool generate_node_acls;
    int login_timeout; // 15,
    int netif_timeout; // 2,
    int prod_mode_write_protect;  // 0
} attributes_t, *attri_t;


typedef struct iscsitarget
{
    attributes_t    attr;
    char chap_mutual_password[MAXCHAPPWD];  //: "aeae2e26-f043-42a7",
    char chap_mutual_userid[MAXCHAPUSER];   // "mutual-rts-user",
    char chap_password[MAXCHAPPWD];         //: "b492785e-bc91-4710",
    char chap_userid[MAXCHAPUSER];          // "rts-user",

    bool         enable;
    char            luns[MAXLUNS][MAXNAMELEN];  // a list of backstores -- "/backstores/fileio/vol4"
    char            node_acls[MAXACLS][MAXNAMELEN]; // a list of initiators -- "iqn.1991-05.com.microsoft:ibm-t410s"; we don't support mapped luns for now.
    parameters_t    param;
    portals_t       portals[MAXPORTALS];
    int             tag;        //always 0; which implies one target has one TPG only.
    char            wwn[MAXWWNLEN]; //specified by user
} iscsitarget_t, *target_t;



port_t test_port()
{
    port_t port = (port_t*)malloc(sizeof(portals_t));
    strcpy(port->ip_address, "192.168.1.55"); 
    port->iser = 1;
    port->port = 3260;
    return port;
}



para_t test_para()
{
    para_t para = (para_t*)malloc(sizeof(parameters_t));
    para->AuthMethod = 2;
    return para;
}


attri_t test_attri()
{
    attri_t attri = (attri_t*)malloc(sizeof(attributes_t));
    attri->authentication = 0;
    attri->cache_dynamic_acls = 1;
    return attri;

}



target_t test_target()
{
    target_t target = (target_t*)malloc(sizeof(iscsitarget_t));
    strcpy(target->chap_mutual_password, "123455432112");
    strcpy(target->chap_mutual_userid, "iqn.2003-01.org.linux-iscsi.localhost.x8664:sn.e16787b68bb4");
    strcpy(target->chap_password, "567899876556");
    strcpy(target->chap_userid, "iqn.1991-05.com.microsoft:ibm-t410s");
    target->enable = 1;
    return target;
}



sobj_t test_sobj()
{
    sobj_t sobj = (sobj_t*)malloc(sizeof(storage_obj_t));
    strcpy(sobj->dev,"/dev/sdd");
    strcpy(sobj->name,"disk_d");
    sobj->plugin = 2;
    sobj->size = 2147483648;
    return sobj;
}
