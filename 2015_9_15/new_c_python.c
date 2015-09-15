#include <stdio.h>
#include <stdlib.h>
#include <python2.7/Python.h>
#include <stdbool.h>
#include <string.h>



#define MAXNAMELEN 100
#define MAXIPLEN 16
#define MAXCHAPPWD 16
#define MAXCHAPUSER 100
#define MAXLUNS 10
#define MAXACLS 100
#define MAXPORTALS 21
#define MAXWWNLEN 100

/*****************************define the struct**************************************/
typedef struct storage_obj
{
    char dev[MAXNAMELEN];  // "/home/admin/sparcedata",
    char name[MAXNAMELEN]; // "vol4",
    int  plugin;            // "fileio", "blockio"
    uint64_t size;          // 2147483648,
    int write_back;         // true,
    char wwn[MAXNAMELEN];    // "dffc64ce-1e34-4410-9531-b71bb7b44848"
} storage_obj_t;


typedef struct portals
{    
    char ip_address[MAXIPLEN];
    int iser;   // true or false
    int port;
} portals_t;



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
    char            luns[MAXLUNS][MAXNAMELEN];  // a list of backstores -- "/backstores/fileio/vol4"
    char            node_acls[MAXACLS][MAXNAMELEN]; // a list of initiators -- "iqn.1991-05.com.microsoft:ibm-t410s"; we don't support mapped luns for now.
    
    int             tag;        //always 1; which implies one target has one TPG only.
    char            wwn[MAXWWNLEN]; //specified by user
} iscsitarget_t;


/*
 *  create and enable an iscsi target
 *
 *  execute sequence:
 *  1)create_block_backstore
 *  2)creat_target
 *  3)create_tpg
 *  4)enable_tpg
 *  5)set_tpg_attr
 *
 *  @sobj   storage obj for the target; create the obj if it doesn't exist
 *  @target iscsi target definition
 *
 *  @return wwn of the storage obj in sobj->wwn if was created
 *          1 success, 0 otherwise.
 */
int creat_block_target(storage_obj_t *sobj, iscsitarget_t *target)
{

    PyObject *pName, *pModule, *pDict, *pFunc, *pArgs, *pRetVal;

    int step_1 = 0, step_2 = 0, step_3 = 0, step_4 = 0;
    
    Py_Initialize();
    if(!Py_IsInitialized())   
    {  
        printf("cannot find rts");
        getchar();
        return -1;  
    }

    PyRun_SimpleString("import sys");
    PyRun_SimpleString("sys.path.append('./')");

    pName = PyString_FromString("rtslib_def"); 
    if ( !pName)
    {
        printf("cannot find rtslibname");
        getchar();
        return -1;
    }

    pModule = PyImport_Import(pName);
    if ( !pModule)
    {
        printf("cannot find rtslib_def.py");
        getchar();
        return -1;

    }

    pDict = PyModule_GetDict(pModule);
    if (!pDict)
    {
        printf("cannot find rtslib_def_dict");
        getchar();
        return -1;
    }

    /*create block bacstore*/
    pFunc = PyDict_GetItemString(pDict,"create_block_backstore");
    if ( !pFunc || !PyCallable_Check(pFunc) )  
    {
        printf("can't find block create_block_backstore");  
        getchar();  
        return -1;  
    }
    pArgs = PyTuple_New(2);
    PyTuple_SetItem(pArgs, 0, Py_BuildValue("s",sobj->dev));
    PyTuple_SetItem(pArgs, 1, Py_BuildValue("s",sobj->name));
    pRetVal = PyObject_CallObject(pFunc, pArgs);
    printf("return 0 for create_block_backstore sucess : %d\n", PyInt_AsLong(pRetVal));
    step_1 = PyInt_AsLong(pRetVal);

    if (step_1 == 0)
    {
        /*create target*/
        pFunc = PyDict_GetItemString(pDict,"create_target");
        if ( !pFunc || !PyCallable_Check(pFunc) )  
        {
            printf("can't find func");  
            getchar();  
            return -1;  
        }

        pRetVal = PyObject_CallObject(pFunc,NULL);
        strcpy(sobj->wwn,PyString_AsString(pRetVal));




        /*creat tpg*/
        pFunc = PyDict_GetItemString(pDict,"create_tpg");
        if ( !pFunc || !PyCallable_Check(pFunc) )  
        {
            printf("can't find block create_tpg");  
            getchar();  
            return -1;  
        }
        pArgs = PyTuple_New(4);
        PyTuple_SetItem(pArgs, 0, Py_BuildValue("s",sobj->name));
        PyTuple_SetItem(pArgs, 1, Py_BuildValue("s",sobj->wwn));
        PyTuple_SetItem(pArgs, 2, Py_BuildValue("s",target->portal->ip_address));
        PyTuple_SetItem(pArgs, 3, Py_BuildValue("i",target->portal->port));
        pRetVal = PyObject_CallObject(pFunc, pArgs);
        printf("return 0 for create_tpg sucess : %d\n", PyInt_AsLong(pRetVal));
        step_2 = PyInt_AsLong(pRetVal);

        /*enable the tpg*/
        pFunc = PyDict_GetItemString(pDict,"enable_tpg");
        if ( !pFunc || !PyCallable_Check(pFunc) )  
        {
            printf("can't find block enable_tpg");  
            getchar();  
            return -1;  
        }
        pArgs = PyTuple_New(3);
        PyTuple_SetItem(pArgs, 0, Py_BuildValue("s",sobj->wwn));
        PyTuple_SetItem(pArgs, 1, Py_BuildValue("i",target->tag));
        PyTuple_SetItem(pArgs, 2, Py_BuildValue("i",target->enable));
        pRetVal = PyObject_CallObject(pFunc, pArgs);
        printf("return 0 for enable_tpg sucess : %d\n", PyInt_AsLong(pRetVal));
        step_3 = PyInt_AsLong(pRetVal);

        /*set the attribute of the tpg*/
        pFunc = PyDict_GetItemString(pDict,"set_tpg_attr");
        if ( !pFunc || !PyCallable_Check(pFunc) )  
        {
            printf("can't find block enable_tpg");  
            getchar();  
            return -1;  
        }
        pArgs = PyTuple_New(10);
        PyTuple_SetItem(pArgs, 0, Py_BuildValue("s",sobj->wwn));
        PyTuple_SetItem(pArgs, 1, Py_BuildValue("i",target->tag));
        PyTuple_SetItem(pArgs, 2, Py_BuildValue("i",target->attrib->authentication));
        PyTuple_SetItem(pArgs, 3, Py_BuildValue("i",target->attrib->cache_dynamic_acls));
        PyTuple_SetItem(pArgs, 4, Py_BuildValue("i",target->attrib->default_cmdsn_depth));
        PyTuple_SetItem(pArgs, 5, Py_BuildValue("i",target->attrib->demo_mode_write_protect));
        PyTuple_SetItem(pArgs, 6, Py_BuildValue("i",target->attrib->generate_node_acls));
        PyTuple_SetItem(pArgs, 7, Py_BuildValue("i",target->attrib->login_timeout));
        PyTuple_SetItem(pArgs, 8, Py_BuildValue("i",target->attrib->netif_timeout));
        PyTuple_SetItem(pArgs, 9, Py_BuildValue("i",target->attrib->prod_mode_write_protect));
        pRetVal = PyObject_CallObject(pFunc, pArgs);
        printf("return 0 for set_tpg_attr sucess : %d\n", PyInt_AsLong(pRetVal));
        step_4 = PyInt_AsLong(pRetVal);

        if (!(step_2||step_3||step_4))
        {
            printf("creat_block_target %s success\n",sobj->wwn);
        
        }
        else
        {
            printf("create target failed so delete the backstore and target\n");
            delete_backstore(sobj->name);
            delete_target(sobj->wwn);
        }
    }
    else
    {
        printf("creat backstore failed\n");

    }

        
    

    //return PyString_AsString(pRetVal);
    //PyArg_Parse(pRetVal, "s", &string )
    //return(string)

    Py_DECREF(pName);
    Py_DECREF(pArgs);
    Py_DECREF(pModule);
    Py_DECREF(pRetVal);

    Py_Finalize();
}

/*
 * remove all targets
 *
 * @return 0 success; -1 otherwise
 */

int delete_all_targets(void)
{
    PyObject *pName, *pModule, *pDict, *pFunc, *pRetVal;  
    
    Py_Initialize();
    if(!Py_IsInitialized())   
    {  
        return -1;  
    }

    PyRun_SimpleString("import sys");
    PyRun_SimpleString("sys.path.append('./')");

    pName = PyString_FromString("rtslib_def"); 
    if ( !pName)
    {
        printf("cannot find rtslibname");
        getchar();
        return -1;
    }

    pModule = PyImport_Import(pName);
    if ( !pModule)
    {
        printf("cannot find rtslib_def.py");
        getchar();
        return -1;

    }

    pDict = PyModule_GetDict(pModule);
    if (!pDict)
    {
        printf("cannot find rtslib_def_dict");
        getchar();
        return -1;
    }

    pFunc = PyDict_GetItemString(pDict,"delete_all_targets");
    if ( !pFunc || !PyCallable_Check(pFunc) )  
    {
        printf("can't find func");  
        getchar();  
        return -1;  
    }

    pRetVal = PyObject_CallObject(pFunc,NULL);
    printf("return 1 for delete success : %d\n", PyInt_AsLong(pRetVal)); 


    Py_DECREF(pName);
    Py_DECREF(pModule);
    Py_DECREF(pRetVal);
    Py_Finalize();
}


/*
 * remove an iscsi target
 *
 * @wwn iscsi target's wwn
 *
 * @return 0 success; -1 otherwise
 */
int delete_target(char wwn[])
{
    PyObject *pName, *pModule, *pDict, *pFunc, *pArgs, *pRetVal;
    
    Py_Initialize();
    if(!Py_IsInitialized())   
    {  
        printf("cannot find rts");
        getchar();
        return -1;  
    }

    PyRun_SimpleString("import sys");
    PyRun_SimpleString("sys.path.append('./')");

    pName = PyString_FromString("rtslib_def"); 
    if ( !pName)
    {
        printf("cannot find rtslibname");
        getchar();
        return -1;
    }

    pModule = PyImport_Import(pName);
    if ( !pModule)
    {
        printf("cannot find rtslib_def.py");
        getchar();
        return -1;

    }

    pDict = PyModule_GetDict(pModule);
    if (!pDict)
    {
        printf("cannot find rtslib_def_dict");
        getchar();
        return -1;
    }

    /*delete the target*/
    pFunc = PyDict_GetItemString(pDict,"delete_target");
    if ( !pFunc || !PyCallable_Check(pFunc) )  
    {
        printf("can't find block delete_target");  
        getchar();  
        return -1;  
    }
    pArgs = PyTuple_New(1);
    PyTuple_SetItem(pArgs, 0, Py_BuildValue("s",wwn));
    pRetVal = PyObject_CallObject(pFunc, pArgs);
    printf("return 0 for delete the target success : %d\n", PyInt_AsLong(pRetVal));

    Py_DECREF(pName);
    Py_DECREF(pArgs);
    Py_DECREF(pModule);
    Py_DECREF(pRetVal);

    Py_Finalize();
}

/*
 * remove an iscsi backstore
 *
 * @wwn iscsi backstore's name
 *
 * @return 0 success; -1 otherwise
 */
int delete_backstore(char name[])
{
    PyObject *pName, *pModule, *pDict, *pFunc, *pArgs, *pRetVal;
    
    Py_Initialize();
    if(!Py_IsInitialized())   
    {  
        printf("cannot find rts");
        getchar();
        return -1;  
    }

    PyRun_SimpleString("import sys");
    PyRun_SimpleString("sys.path.append('./')");

    pName = PyString_FromString("rtslib_def"); 
    if ( !pName)
    {
        printf("cannot find rtslibname");
        getchar();
        return -1;
    }

    pModule = PyImport_Import(pName);
    if ( !pModule)
    {
        printf("cannot find rtslib_def.py");
        getchar();
        return -1;

    }

    pDict = PyModule_GetDict(pModule);
    if (!pDict)
    {
        printf("cannot find rtslib_def_dict");
        getchar();
        return -1;
    }

    /*delete the target*/
    pFunc = PyDict_GetItemString(pDict,"delete_backstore");
    if ( !pFunc || !PyCallable_Check(pFunc) )  
    {
        printf("can't find block delete_backstore");  
        getchar();  
        return -1;  
    }
    pArgs = PyTuple_New(1);
    PyTuple_SetItem(pArgs, 0, Py_BuildValue("s",name));
    pRetVal = PyObject_CallObject(pFunc, pArgs);
    printf("return 0 for delete the delete_backstore success : %d\n", PyInt_AsLong(pRetVal));

    Py_DECREF(pName);
    Py_DECREF(pArgs);
    Py_DECREF(pModule);
    Py_DECREF(pRetVal);

    Py_Finalize();
}



/*
 * Set mutual authentication credential for an iscsi target
 * @wwn     iscsi target's wwn
 * @mutual_user mutual_passwd user passwd    iscsi target'auth parameter
 * 
 * @Note: The Microsoft Windows iSCSI Initiator uses its IQN as a default login name. It
 * requires the password length to be at least 12 bytes (96 bits), and it rejects passwords that
 * are too simple.
 *
 *
 * @return 1 success 0 otherwise
 *
 *
 *
 */
int set_mutual_auth(char wwn[], int tag, char mutual_user[], char mutual_passwd[],char user[], char passwd[])
{
    PyObject *pName, *pModule, *pDict, *pFunc, *pArgs, *pRetVal;
    
    Py_Initialize();
    if(!Py_IsInitialized())   
    {  
        printf("cannot find rts");
        getchar();
        return -1;  
    }

    PyRun_SimpleString("import sys");
    PyRun_SimpleString("sys.path.append('./')");

    pName = PyString_FromString("rtslib_def"); 
    if ( !pName)
    {
        printf("cannot find rtslibname");
        getchar();
        return -1;
    }

    pModule = PyImport_Import(pName);
    if ( !pModule)
    {
        printf("cannot find rtslib_def.py");
        getchar();
        return -1;

    }

    pDict = PyModule_GetDict(pModule);
    if (!pDict)
    {
        printf("cannot find rtslib_def_dict");
        getchar();
        return -1;
    }

    /*set TPG mutual_auth*/
    pFunc = PyDict_GetItemString(pDict,"set_TPG_mutual_auth");
    if ( !pFunc || !PyCallable_Check(pFunc) )  
    {
        printf("can't find block delete_target");  
        getchar();  
        return -1;  
    }
    pArgs = PyTuple_New(6);
    PyTuple_SetItem(pArgs, 0, Py_BuildValue("s",wwn));
    PyTuple_SetItem(pArgs, 1, Py_BuildValue("i",tag));
    PyTuple_SetItem(pArgs, 2, Py_BuildValue("s",mutual_user));
    PyTuple_SetItem(pArgs, 3, Py_BuildValue("s",mutual_passwd));
    PyTuple_SetItem(pArgs, 4, Py_BuildValue("s",user));
    PyTuple_SetItem(pArgs, 5, Py_BuildValue("s",passwd));
    pRetVal = PyObject_CallObject(pFunc, pArgs);
    printf("return 1 for set_TPG_mutual_auth success : %d\n", PyInt_AsLong(pRetVal));

    Py_DECREF(pName);
    Py_DECREF(pArgs);
    Py_DECREF(pModule);
    Py_DECREF(pRetVal);

    Py_Finalize();
}

/*
 * Set initiator ACLs for an iscsi target
 *
 * @wwn     iscsi target's WWN
 * @tag     tpg's index
 * @node_acls   an array of iscsi initiator's IQNs
 * @number      size of inittiator array
 *
 * @return  1 success 0 otherwise
*/
int set_initiators(char wwn[], int tag, char node_acls[][MAXNAMELEN], int number)
{
    PyObject *pName, *pModule, *pDict, *pFunc, *pArgs, *pRetVal;
    
    Py_Initialize();
    if(!Py_IsInitialized())   
    {  
        printf("cannot find rts");
        getchar();
        return -1;  
    }

    PyRun_SimpleString("import sys");
    PyRun_SimpleString("sys.path.append('./')");

    pName = PyString_FromString("rtslib_def"); 
    if ( !pName)
    {
        printf("cannot find rtslibname");
        getchar();
        return -1;
    }

    pModule = PyImport_Import(pName);
    if ( !pModule)
    {
        printf("cannot find rtslib_def.py");
        getchar();
        return -1;

    }

    pDict = PyModule_GetDict(pModule);
    if (!pDict)
    {
        printf("cannot find rtslib_def_dict");
        getchar();
        return -1;
    }

    /*set initiator list*/
    pFunc = PyDict_GetItemString(pDict,"set_initiator");
    if ( !pFunc || !PyCallable_Check(pFunc) )  
    {
        printf("can't find block set_initiator");  
        getchar();  
        return -1;  
    }
    int i ;
    for (i = 0; i < number; ++i)
    {
        pArgs = PyTuple_New(3);
        PyTuple_SetItem(pArgs, 0, Py_BuildValue("s",wwn));
        PyTuple_SetItem(pArgs, 1, Py_BuildValue("i",tag));
        PyTuple_SetItem(pArgs, 2, Py_BuildValue("s",node_acls[i]));
        pRetVal = PyObject_CallObject(pFunc, pArgs);
        if (PyInt_AsLong(pRetVal))
        {
             printf ("set_initiator success\n") ;
        }else
        {
            delete_initiators(wwn, tag, node_acls, i);

        }

    }

    
    Py_DECREF(pName);
    Py_DECREF(pArgs);
    Py_DECREF(pModule);
    Py_DECREF(pRetVal);

    Py_Finalize();
}

/*
 * remove initiator ACLs for an iscsi target
 *
 * @wwn     iscsi target's WWN
 * @tag     tpg's index
 * @node_acls   an array of iscsi initiator's IQNs
 * @number      size of inittiator array
 *
 * @return  1 success 0 otherwise
*/
int delete_initiators(char wwn[], int tag, char node_acls[][MAXNAMELEN], int number)
{
    PyObject *pName, *pModule, *pDict, *pFunc, *pArgs, *pRetVal;
    
    Py_Initialize();
    if(!Py_IsInitialized())   
    {  
        printf("cannot find rts");
        getchar();
        return -1;  
    }

    PyRun_SimpleString("import sys");
    PyRun_SimpleString("sys.path.append('./')");

    pName = PyString_FromString("rtslib_def"); 
    if ( !pName)
    {
        printf("cannot find rtslibname");
        getchar();
        return -1;
    }

    pModule = PyImport_Import(pName);
    if ( !pModule)
    {
        printf("cannot find rtslib_def.py");
        getchar();
        return -1;

    }

    pDict = PyModule_GetDict(pModule);
    if (!pDict)
    {
        printf("cannot find rtslib_def_dict");
        getchar();
        return -1;
    }

    /*delete the initiator */
    pFunc = PyDict_GetItemString(pDict,"delete_initiator");
    if ( !pFunc || !PyCallable_Check(pFunc) )  
    {
        printf("can't find block set_initiator");  
        getchar();  
        return -1;  
    }
    int i ;
    for (i = 0; i < number; ++i)
    {
        pArgs = PyTuple_New(3);
        PyTuple_SetItem(pArgs, 0, Py_BuildValue("s",wwn));
        PyTuple_SetItem(pArgs, 1, Py_BuildValue("i",tag));
        PyTuple_SetItem(pArgs, 2, Py_BuildValue("s",node_acls[i]));
        pRetVal = PyObject_CallObject(pFunc, pArgs);
        if (PyInt_AsLong(pRetVal))
        {
            printf ("delete_initiator success\n") ;
        }
        else
        {
            printf ("delete_initiator failed\n");

        }


    }

    
    

    Py_DECREF(pName);
    Py_DECREF(pArgs);
    Py_DECREF(pModule);
    Py_DECREF(pRetVal);

    Py_Finalize();

}



int main(int argc, char* argv[])
{


    delete_all_targets();


/*
*testcase 1
* create sucess
*
*/
    /* initialize the target */
    iscsitarget_t *target;
    target = (iscsitarget_t*)malloc(sizeof(iscsitarget_t));
    strcpy(target->chap_mutual_password, "567899876556");
    strcpy(target->chap_mutual_userid, "iqn.1991-05.com.microsoft:ibm-t410s");
    strcpy(target->chap_password, "123455432112");
    strcpy(target->chap_userid, "iqn.2003-01.org.linux-iscsi.localhost.x8664:sn.e16787b68bb4");
    target->enable = 1;
    target->tag = 1;

    /* initialize the storeobject */
    storage_obj_t *sobj;
    sobj = (storage_obj_t*)malloc(sizeof(storage_obj_t));
    strcpy(sobj->dev,"/dev/sdc");
    strcpy(sobj->name,"disk_c");
    sobj->plugin = 2;
    sobj->size = 2147483648;

    /* initialize the portals */
    portals_t *portal;
    target->portal = (portals_t*)malloc(sizeof(portals_t));
    strcpy(target->portal->ip_address, "192.168.1.77"); 
    target->portal->iser = 1;
    target->portal->port = 3260;

    /* initialize the parameters */
    parameters_t *parame;
    target->parame = (parameters_t*)malloc(sizeof(parameters_t));
    target->parame->AuthMethod = 2;

    /* initialize the attributes */
    attributes_t *attrib;
    target->attrib = (attributes_t*)malloc(sizeof(attributes_t));
    target->attrib->authentication = 0;
    target->attrib->cache_dynamic_acls = 1;
    target->attrib->default_cmdsn_depth = 16;
    target->attrib->demo_mode_write_protect = 0;
    target->attrib->generate_node_acls = 1;
    target->attrib->login_timeoutservice iscsid start = 15;
    target->attrib->netif_timeout = 2;
    target->attrib->prod_mode_write_protect = 0;
    printf("testcase1\n");
    creat_block_target(sobj, target);

    /* this block is create for testcase 6 */
    char auth_wwn[MAXNAMELEN];
    char auth_mutual_user[MAXCHAPUSER];
    char auth_mutual_passwd[MAXCHAPPWD];
    char auth_user[MAXCHAPUSER];
    char auth_passwd[MAXCHAPPWD];
    strcpy(auth_wwn, sobj->wwn); 
    strcpy(auth_mutual_passwd, target->chap_mutual_password);
    strcpy(auth_mutual_user, target->chap_mutual_userid);
    strcpy(auth_passwd, target->chap_password);
    strcpy(auth_user, target->chap_userid);



    /* free the memory */
    free(target);
    free(sobj);



// /*
// * testcase 2
// * backstore exist 
// *
// */
//     /* initialize the target */
//     iscsitarget_t *target1;
//     target1 = (iscsitarget_t*)malloc(sizeof(iscsitarget_t));
//     strcpy(target1->chap_mutual_password, "123455432112");
//     strcpy(target1->chap_mutual_userid, "iqn.2003-01.org.linux-iscsi.localhost.x8664:sn.e16787b68bb4");
//     strcpy(target1->chap_password, "567899876556");
//     strcpy(target1->chap_userid, "iqn.1991-05.com.microsoft:ibm-t410s");
//     target1->enable = 1;
//     target1->tag = 1;

//     /* initialize the storeobject */
//     storage_obj_t *sobj1;
//     sobj1 = (storage_obj_t*)malloc(sizeof(storage_obj_t));
//     strcpy(sobj1->dev,"/dev/sdb");
//     strcpy(sobj1->name,"disk_b");
//     sobj1->plugin = 2;
//     sobj1->size = 2147483648;

//     /* initialize the portals */
//     //portals_t *portal;
//     target1->portal = (portals_t*)malloc(sizeof(portals_t));
//     strcpy(target1->portal->ip_address, "192.168.1.77"); 
//     target1->portal->iser = 1;
//     target1->portal->port = 3260;

//     /* initialize the parameters */
//     //parameters_t *parame1;
//     target1->parame = (parameters_t*)malloc(sizeof(parameters_t));
//     target1->parame->AuthMethod = 2;

//     /* initialize the attributes */
//     target1->attrib = (attributes_t*)malloc(sizeof(attributes_t));
//     target1->attrib->authentication = 0;
//     target1->attrib->cache_dynamic_acls = 1;
//     target1->attrib->default_cmdsn_depth = 16;
//     target1->attrib->demo_mode_write_protect = 0;
//     target1->attrib->generate_node_acls = 1;
//     target1->attrib->login_timeout = 15;
//     target1->attrib->netif_timeout = 2;
//     target1->attrib->prod_mode_write_protect = 0;

//     printf("testcase2\n");
//     creat_block_target(sobj1, target1);

//     /* free the memory */
//     free(target1);
//     free(sobj1);

// /*
// * testcase 3 
// * create tpg failed 
// *
// * @method set ip_adress uncorrect 
// *
// *
// *
// */
//     iscsitarget_t *target2;
//     target2 = (iscsitarget_t*)malloc(sizeof(iscsitarget_t));
//     strcpy(target2->chap_mutual_password, "123455432112");
//     strcpy(target2->chap_mutual_userid, "iqn.2003-01.org.linux-iscsi.localhost.x8664:sn.e16787b68bb4");
//     strcpy(target2->chap_password, "567899876556");
//     strcpy(target2->chap_userid, "iqn.1991-05.com.microsoft:ibm-t410s");
//     target2->enable = 1;
//     target2->tag = 1;

//     /* initialize the storeobject */
//     storage_obj_t *sobj2;
//     sobj2 = (storage_obj_t*)malloc(sizeof(storage_obj_t));
//     strcpy(sobj2->dev,"/dev/sdc");
//     strcpy(sobj2->name,"disk_c");
//     sobj2->plugin = 2;
//     sobj2->size = 2147483648;

//     /* initialize the portals */
//     target2->portal = (portals_t*)malloc(sizeof(portals_t));
//     strcpy(target2->portal->ip_address, ""); 
//     target2->portal->iser = 1;
//     target2->portal->port = 3260;

//     /* initialize the parameters */
//     target2->parame = (parameters_t*)malloc(sizeof(parameters_t));
//     target2->parame->AuthMethod = 2;

//     /* initialize the attributes */
//     target2->attrib = (attributes_t*)malloc(sizeof(attributes_t));
//     target2->attrib->authentication = 0;
//     target2->attrib->cache_dynamic_acls = 1;
//     target2->attrib->default_cmdsn_depth = 16;
//     target2->attrib->demo_mode_write_protect = 0;
//     target2->attrib->generate_node_acls = 1;
//     target2->attrib->login_timeout = 15;
//     target2->attrib->netif_timeout = 2;
//     target2->attrib->prod_mode_write_protect = 0;
//     printf("testcase3\n");
//     creat_block_target(sobj2, target2);

//     /* free the memory */
//     free(target2);
//     free(sobj2);


// /* testcase 4 
// * set tpg attribute failed 
// *
// * @method not set the netif_timeout  
// *
// *
// *
// */
//     iscsitarget_t *target3;
//     target3 = (iscsitarget_t*)malloc(sizeof(iscsitarget_t));
//     strcpy(target3->chap_mutual_password, "123455432112");
//     strcpy(target3->chap_mutual_userid, "iqn.2003-01.org.linux-iscsi.localhost.x8664:sn.e16787b68bb4");
//     strcpy(target3->chap_password, "567899876556");
//     strcpy(target3->chap_userid, "iqn.1991-05.com.microsoft:ibm-t410s");
//     target3->enable = 1;
//     target3->tag = 1;

//     /* initialize the storeobject */
//     storage_obj_t *sobj3;
//     sobj3 = (storage_obj_t*)malloc(sizeof(storage_obj_t));
//     strcpy(sobj3->dev,"/dev/sdc");
//     strcpy(sobj3->name,"disk_c");
//     sobj3->plugin = 2;
//     sobj3->size = 2147483648;

//     /* initialize the portals */
//    // portals_t *portal;
//     target3->portal = (portals_t*)malloc(sizeof(portals_t));
//     strcpy(target3->portal->ip_address, "192.168.1.77"); 
//     target3->portal->iser = 1;
//     target3->portal->port = 3260;

//     /* initialize the parameters */
//     //parameters_t *parame1;
//     target3->parame = (parameters_t*)malloc(sizeof(parameters_t));
//     target3->parame->AuthMethod = 2;

//     /* initialize the attributes */
//    // attributes_t *attrib;
//     target3->attrib = (attributes_t*)malloc(sizeof(attributes_t));
//     target3->attrib->authentication = 1;
//     target3->attrib->cache_dynamic_acls = 1;
//     target3->attrib->default_cmdsn_depth = 16;
//     target3->attrib->demo_mode_write_protect = 0;
//     target3->attrib->generate_node_acls = 1;
//     target3->attrib->login_timeout = 15;
//     //target3->attrib->netif_timeout = 2;
//     target3->attrib->prod_mode_write_protect = 0;
//     printf("testcase4\n");
//     creat_block_target(sobj3, target3);

//     /* free the memory */
//     free(target3);
//     free(sobj3);

// /* testcase 5 
// * delete the target which can not found
// *
// * @method set the wwn by accident
// *  
// * @Note:delete_target has been proved in testcase3 and testcase4
// *
// */
//     printf("testcase5\n");
//     char wwn[MAXNAMELEN] = "iqn.2003-01.org.linux-iscsi.localhost.x8664:sn.2cb54bf3e239";
//     delete_target(wwn);


// /* testcase 6 
// * set the mutual authentition
// *
// * @method set the authentition 
// * 
// *  
// * @note we create some paranmeter in testcase 1 to transfer here
// *
// */

//     char wwn_auth[MAXNAMELEN];
//     int tag = 1; 
//     char mutual_user[MAXCHAPUSER];
//     char mutual_passwd[MAXCHAPPWD];
//     char user[MAXCHAPUSER];
//     char passwd[MAXCHAPPWD]; 
//     strcpy(wwn_auth, auth_wwn);
//     strcpy(mutual_passwd, auth_mutual_passwd);
//     strcpy(mutual_user, auth_mutual_user);
//     strcpy(passwd, auth_passwd);
//     strcpy(user, auth_user);
//     printf("testcase6\n");
//     set_mutual_auth(wwn_auth, tag,  mutual_user, mutual_passwd, user, passwd);



/* testcase 7 
* set the  initiator ACLs for an iscsi target
*
* @method set the authentition 
* 
*  
* @note we create some paranmeter in testcase 1 to transfer here
*
*/  

    int tag = 1;
    char wwn_acl[MAXNAMELEN];
    //char wwn[MAXNAMELEN] = "iqn.2003-01.org.linux-iscsi.localhost.x8664:sn.2cb54bf3e239";
    strcpy(wwn_acl, auth_wwn);
    char node_acls[MAXACLS][MAXNAMELEN] = {"iqn.1991-05.com.microsoft:ibm-t410s","iqn.1994-05.com.redhat:a4bf13a296c"};
    int number = 2;
    printf("testcase7\n");
    set_initiators(wwn_acl, tag, node_acls, number);



}
