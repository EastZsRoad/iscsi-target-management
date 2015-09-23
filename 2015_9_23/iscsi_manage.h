
#include <stdbool.h>
#include <stdarg.h>


#define MAXNAMELEN 100
#define MAXIPLEN 16
#define MAXCHAPPWD 16
#define MAXCHAPUSER 100
#define MAXLUNS 10
#define MAXACLS 100
#define MAXPORTALS 21
#define MAXWWNLEN 256

enum iscsi_plugin {FILEIO = 1,BLOCK = 2};
enum iscsi_AuthMethod {CHAP = 1,NONE = 2};

/*****************************define the struct**************************************/
typedef struct storage_obj
{
    char dev[MAXNAMELEN];  // "/home/admin/sparcedata",
    char name[MAXNAMELEN]; // "vol4",
    int  iscsi_plugin;            // 1 for "fileio", 2 for "blockio"
    uint64_t size ;          // 2147483648,
    int write_back;         // true,
    
} storage_obj_t;


typedef struct portals
{    
    char ip_address[MAXIPLEN];
    int iser;   // true or false
    int port;
} portals_t;



typedef struct  parameters
{
    int iscsi_AuthMethod;   // "CHAP = 1,None = 2", "CHAP mutual = 3", etc
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
} parameters_t;



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
} attributes_t;



typedef struct iscsitarget
{
    attributes_t    *attrib;
    parameters_t    *parame;
    portals_t       *portal;
    char chap_mutual_password[MAXCHAPPWD];  //: "aeae2e26-f043-42a7",
    char chap_mutual_userid[MAXCHAPUSER];   // "mutual-rts-user",
    char chap_password[MAXCHAPPWD];         //: "b492785e-bc91-4710",
    char chap_userid[MAXCHAPUSER];          // "rts-user",
    
    bool         enable;
    //char            luns[MAXLUNS][MAXNAMELEN];  // a list of backstores -- "/backstores/fileio/vol4"
    char            node_acls[MAXACLS][MAXNAMELEN]; // a list of initiators -- "iqn.1991-05.com.microsoft:ibm-t410s"; we don't support mapped luns for now.
    int             node_acls_num ;
    
    int             tag;        //always 1; which implies one target has one TPG only.
    char            wwn[MAXWWNLEN]; //specified by user

} iscsitarget_t;


// int iscsi_create_block_target(storage_obj_t *sobj, iscsitarget_t *target);
// int iscsi_create_fileio_target(storage_obj_t *sobj, iscsitarget_t *target);

int iscsi_create_target(storage_obj_t *sobj, iscsitarget_t *target);
int iscsi_del_all_targets(void);
int iscsi_del_target(bool del_back, char wwn[]);
int iscsi_set_ACLs_auth(char wwn[], int tag, char acl_wwn[][MAXNAMELEN],int number, char mutual_user[], char mutual_passwd[],char user[], char passwd[]);
int iscsi_del_ACLs_auth(char wwn[], int tag);
int iscsi_set_initiators(char wwn[], int tag, char node_acls[][MAXNAMELEN], int number);
int iscsi_del_initiators(char wwn[], int tag, char node_acls[][MAXNAMELEN], int number);
//int iscsi_del_all_initiators(char wwn[], int tag);
//int iscsi_set_mutual_auth(char wwn[], int tag, char mutual_user[], char mutual_passwd[],char user[], char passwd[]);
//int iscsi_del_mutual_auth(char wwn[], int tag);
