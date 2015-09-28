#include <stdio.h>
#include <stdlib.h>
#include <python2.7/Python.h>
#include <stdbool.h>
#include <string.h>

#include "iscsi_manage.h"

#define MAXLUNS 10
#define MAXNAMELEN 100


portals_t *portal;
parameters_t *parame;
attributes_t *attrib;
iscsitarget_t *target;
storage_obj_t *sobj;



void show_targetcli()
{
    Py_Initialize();
    PyRun_SimpleString("import os");
    PyRun_SimpleString("os.system('targetcli ls')");
    Py_Finalize(); 
    return;  

}


/* initialize the target */
int init_target()
{
    // iscsitarget_t *target;
    target = (iscsitarget_t*)malloc(sizeof(iscsitarget_t));
    strcpy(target->chap_mutual_password, "567899876556");
    strcpy(target->chap_mutual_userid, "iqn.1991-05.com.microsoft:ibm-t410s");
    strcpy(target->chap_password, "123455432112");
    strcpy(target->chap_userid, "iqn.2003-01.org.linux-iscsi.localhost.x8664:sn.e16787b68bb4");
    target->enable = 1;
    target->tag = 1;
    target->node_acls_num = 2;
    strcpy((target->node_acls)[0],"iqn.1991-05.com.microsoft:ibm-t410s");
    strcpy((target->node_acls)[1],"iqn.1994-05.com.redhat:a4bf13a296c");
    strcpy(target->wwn, "iqn.2003-01.org.linux-iscsi.localhost.x8664:sn.e16787b68bb4");
    return 0;
}

/* initialize the storeobject */
int init_sobj()
{
    // storage_obj_t *sobj;
    sobj = (storage_obj_t*)malloc(sizeof(storage_obj_t));
    strcpy(sobj->dev,"/dev/sdc");
    strcpy(sobj->name,"disk_c");
    sobj->iscsi_plugin = 2;
    sobj->size = 2147483648;
    return 0;
}

/* initialize the portals */
int init_portal()
{
    target->portal = (portals_t*)malloc(sizeof(portals_t));
    strcpy(target->portal->ip_address, "192.168.1.77"); 
    target->portal->iser = 1;
    target->portal->port = 3260;
    return 0;
}

/* initialize the parameters */
int init_para()
{
    target->parame = (parameters_t*)malloc(sizeof(parameters_t));
    //target->parame->AuthMethod = ;
    return 0;
}

/* initialize the attributes */
int init_attr()
{
    // target = (iscsitarget_t*)malloc(sizeof(iscsitarget_t));
    target->attrib = (attributes_t*)malloc(sizeof(attributes_t));
    target->attrib->authentication = 0;
    target->attrib->cache_dynamic_acls = 1;
    target->attrib->default_cmdsn_depth = 16;
    target->attrib->demo_mode_write_protect = 0;
    target->attrib->generate_node_acls = 1;
    target->attrib->login_timeout = 15;
    target->attrib->netif_timeout = 2;
    target->attrib->prod_mode_write_protect = 0;

    return 0;
}

/* free the memory */
int free_all()
{
    free(target->portal);
    free(target->parame);
    free(target->attrib);
    free(target);
    free(sobj);
    return 0;
}





int main(int argc, char* argv[]) 
 {

// /****************************Success Test*******************************/

// /*testcase 1
// * create target success
// *
// *
// * @result windows initiator in acls could connected
// */
//     iscsi_del_all_targets();
//     init_target();
//     init_sobj();
//     init_portal();
//     init_para();
//     init_attr();

//     printf("Testcase1\n");
//     iscsi_create_target(sobj, target);

//     show_targetcli();
//     free_all();


// /*testcase 2
// * set ACLs mutual auth 
// *
// *
// * @result windows initiator in acls could connected in mutual chap
// * learn more from RTSlib_Test.doc in Z:\test-tools\RTSlib&iscsi\RTSlib_Test
// */
//     iscsi_del_all_targets();
//     init_target();
//     init_sobj();
//     init_portal();
//     init_para();
//     init_attr();

//     printf("Testcase2\n");
//     iscsi_create_target(sobj, target);
//     iscsi_set_ACLs_auth(target->wwn, target->tag, target->node_acls, target->node_acls_num, target->chap_mutual_userid, target->chap_mutual_password, target->chap_userid, target->chap_password);
    
//     show_targetcli();
//     free_all();

// /*testcase 3 
// * delete the ACLs mutual auth  
// *
// *
// * @result windows initiator in acls could connected
// */
//     iscsi_del_all_targets();
//     init_target();
//     init_sobj();
//     init_portal();
//     init_para();
//     init_attr();

//     printf("Testcase3\n");
//     iscsi_create_target(sobj, target);
//     iscsi_set_ACLs_auth(target->wwn, target->tag, target->node_acls, target->node_acls_num, target->chap_mutual_userid, target->chap_mutual_password, target->chap_userid, target->chap_password);
//     iscsi_del_ACLs_auth(target->wwn, target->tag);

//     show_targetcli();
//     free_all();


// /* testcase 4-1
// * delete the target of wwn whihout the target
// *
// * @result backstore do not delete
// */
//     iscsi_del_all_targets();
//     init_target();
//     init_sobj();
//     init_portal();
//     init_para();
//     init_attr();

//     printf("Testcase4-1\n");
//     iscsi_create_target(sobj, target);
//     iscsi_del_target(false,target->wwn);

//     show_targetcli();
//     free_all();

// /* testcase 4-2 
// * delete the target of wwn with the target
// *
// */
//     iscsi_del_all_targets();
//     init_target();
//     init_sobj();
//     init_portal();
//     init_para();
//     init_attr();

//     printf("Testcase4-2\n");
//     iscsi_create_target(sobj, target);
//     iscsi_del_target(true,target->wwn);

//     show_targetcli();
//     free_all();

// /* testcase 5 
// *create fileio target
// *
// * @parameter change
// *   sobj = (storage_obj_t*)malloc(sizeof(storage_obj_t));
// *   strcpy(sobj->dev,"/root/Desktop/LibreOffice_4.4.4_Win_x86.msi");//214 MB(224,423,936)
// *   strcpy(sobj->name,"LibreOffice_4");
// *   sobj->iscsi_plugin = 1;
// *   sobj->size = 12345;
// *
// */  iscsi_del_all_targets();

//     /* initialize the storeobject */
//     sobj = (storage_obj_t*)malloc(sizeof(storage_obj_t));
//     strcpy(sobj->dev,"/root/Desktop/LibreOffice_4.4.4_Win_x86.msi");
//     strcpy(sobj->name,"LibreOffice_4");
//     sobj->iscsi_plugin = 1;
//     sobj->size = 12345;

//     init_target();
//     init_portal();
//     init_para();
//     init_attr();

//     printf("Testcase5\n");
//     iscsi_create_target(sobj, target);

//     show_targetcli();
//     free_all();
    


// /****************************Error Test*******************************/

// /* testcase 6
// *create backstore whith file error 
// *
// *
// * @method input fileiosize is larger than real fileiosize
// *  
// *
// * @result create the file with the input size
// */
//     iscsi_del_all_targets();

//     /* initialize the storeobject */
//     sobj = (storage_obj_t*)malloc(sizeof(storage_obj_t));
//     strcpy(sobj->dev,"/root/Desktop/LibreOffice_4.4.4_Win_x86.msi");//214m(224,423,936)
//     strcpy(sobj->name,"LibreOffice_4");
//     sobj->iscsi_plugin = 1;
//     sobj->size = 53687091200;

//     init_target();
//     init_portal();
//     init_para();
//     init_attr();

//     printf("Testcase6\n");
//     iscsi_create_target(sobj, target);

//     show_targetcli();
//     free_all();

// /* testcase 7
// *create backstore whith file error
// *
// *
// * @method input fileiosize is 0
// *  
// *
// * @result return  file with size 0
// */
//     iscsi_del_all_targets();

//     /* initialize the storeobject */
//     sobj = (storage_obj_t*)malloc(sizeof(storage_obj_t));
//     strcpy(sobj->dev,"/root/Desktop/LibreOffice_4.4.4_Win_x86.msi");//214m(224,423,936)
//     strcpy(sobj->name,"LibreOffice_4");
//     sobj->iscsi_plugin = 1;
//     sobj->size = 0;//

//     init_target();
//     init_portal();
//     init_para();
//     init_attr();

//     printf("Testcase7\n");
//     iscsi_create_target(sobj, target);

//     show_targetcli();
//     free_all();

// /* testcase 8
// *create backstore whith file error
// *
// *
// * @method give filesize -2
// *  
// *
// * @result  no thing establshed
// */
//     iscsi_del_all_targets();

//     /* initialize the storeobject */
//     sobj = (storage_obj_t*)malloc(sizeof(storage_obj_t));
//     strcpy(sobj->dev,"/root/Desktop/iscsitest12");//214m(224,423,936)
//     strcpy(sobj->name,"test12");
//     sobj->iscsi_plugin =1;
//     sobj->size =-2;
    

//     init_target();
//     init_portal();
//     init_para();
//     init_attr();

//     printf("Testcase8\n");
//     iscsi_create_target(sobj, target);

//     //iscsi_create_target(sobj, target);

//     show_targetcli();
//     free_all();



// /* testcase 8-2
// *create backstore whith file error
// *
// *
// * @method give the file path but put the plugin in to 2
// *  
// *
// * @result  Device is not a TYPE_DISK block device
// */
//     iscsi_del_all_targets();

//     /* initialize the storeobject */
//     sobj = (storage_obj_t*)malloc(sizeof(storage_obj_t));
//     strcpy(sobj->dev,"/root/Desktop/iscsitest12");//214m(224,423,936)
//     strcpy(sobj->name,"test12");
//     sobj->iscsi_plugin = 2;
    

//     init_target();
//     init_portal();
//     init_para();
//     init_attr();

//     printf("Testcase8-1\n");
//     iscsi_create_target(sobj, target);

//     //iscsi_create_target(sobj, target);

//     show_targetcli();
//     free_all();

/* testcase 8-3
*create backstore whith file error
*
*
* @method don not give the iqn
*  
*
* @result  WWN not valid as: iqn, naa, eui nothing esablish
*/
    iscsi_del_all_targets();


    init_sobj();

    target = (iscsitarget_t*)malloc(sizeof(iscsitarget_t));
    strcpy(target->chap_mutual_password, "567899876556");
    strcpy(target->chap_mutual_userid, "iqn.1991-05.com.microsoft:ibm-t410s");
    strcpy(target->chap_password, "123455432112");
    strcpy(target->chap_userid, "iqn.2003-01.org.linux-iscsi.localhost.x8664:sn.e16787b68bb4");
    target->enable = 1;
    target->tag = 1;
    target->node_acls_num = 2;
    strcpy((target->node_acls)[0],"iqn.1991-05.com.microsoft:ibm-t410s");
    strcpy((target->node_acls)[1],"iqn.1994-05.com.redhat:a4bf13a296c");
    //strcpy(target->wwn, "iqn.2003-01.org.linux-iscsi.localhost.x8664:sn.e16787b68bb4");


    init_portal();
    init_para();
    init_attr();

    printf("Testcase8-3\n");
    iscsi_create_target(sobj, target);

    //iscsi_create_target(sobj, target);

    show_targetcli();
    free_all();


// /* testcase 9
// *create backstore whith file error
// *
// *
// * @method input file not exist and do not give the size
// *  
// *
// * @result no size created but the object still exist
// */
//     iscsi_del_all_targets();

//     /* initialize the storeobject */
//     sobj = (storage_obj_t*)malloc(sizeof(storage_obj_t));
//     strcpy(sobj->dev,"/root/Desktop/not_exist");//214m(224,423,936)
//     strcpy(sobj->name,"LibreOffice_4");
//     sobj->iscsi_plugin = 1;
//     //sobj->size = 0;//

//     init_target();
//     init_portal();
//     init_para();
//     init_attr();

//     printf("Testcase9\n");
//     iscsi_create_target(sobj, target);

//     show_targetcli();
//     free_all();
    
// /* testcase 10
// *create backstore whith file error
// *
// *
// * @method input file not exist and give the size
// *  
// *
// * @result create the file with the size given
// */
//     iscsi_del_all_targets();

//     /* initialize the storeobject */
//     sobj = (storage_obj_t*)malloc(sizeof(storage_obj_t));
//     strcpy(sobj->dev,"/root/Desktop/not_exist");//214m(224,423,936)
//     strcpy(sobj->name,"not_exist");
//     sobj->iscsi_plugin = 1;
//     sobj->size = 536870912;//

//     init_target();
//     init_portal();
//     init_para();
//     init_attr();

//     printf("Testcase10\n");
//     iscsi_create_target(sobj, target);

//     show_targetcli();
//     free_all();

// /* testcase 11
// *create backstore whith file error
// *
// *
// * @method input file but the iscsi_plugin is block
// *  
// *
// * @result  Device is not a TYPE_DISK block device.
// * nothing establishes
// */
//     iscsi_del_all_targets();

//     /* initialize the storeobject */
//     sobj = (storage_obj_t*)malloc(sizeof(storage_obj_t));
//     strcpy(sobj->dev,"/root/Desktop/LibreOffice_4.4.4_Win_x86.msi");//214m(224,423,936)
//     strcpy(sobj->name,"LibreOffice_4");
//     sobj->iscsi_plugin = 2;
//     sobj->size = 536870912;//

//     init_target();
//     init_portal();
//     init_para();
//     init_attr();

//     printf("Testcase11\n");
//     iscsi_create_target(sobj, target);

//     show_targetcli();
//     free_all();


// /* testcase 12
// *create backstore whith file error
// *
// *
// * @method input error filepath 
// *  
// * @result [Errno 2] No such file or directory
// * nothing established
// */
//     iscsi_del_all_targets();

//     /* initialize the storeobject */
//     sobj = (storage_obj_t*)malloc(sizeof(storage_obj_t));
//     strcpy(sobj->dev,"dev/sdb");
//     strcpy(sobj->name,"disk_b");
//     sobj->iscsi_plugin = 1;
//     sobj->size = 536870912;//512m

//     init_target();
//     init_portal();
//     init_para();
//     init_attr();

//     printf("Testcase12\n");
//     iscsi_create_target(sobj, target);

//     show_targetcli();
//     free_all();


// /* testcase 13
// *create backstore whith block error
// *
// *
// * @method input error block path 
// *  
// * @result [Errno 2] No such file or directory
// * nothing established
// */
//     iscsi_del_all_targets();

//     /* initialize the storeobject */
//     sobj = (storage_obj_t*)malloc(sizeof(storage_obj_t));
//     strcpy(sobj->dev,"dev/sdb");
//     strcpy(sobj->name,"disk_b");
//     sobj->iscsi_plugin = 2;
//     sobj->size = 536870912;//512m

//     init_target();
//     init_portal();
//     init_para();
//     init_attr();

//     printf("Testcase13\n");
//     iscsi_create_target(sobj, target);

//     show_targetcli();
//     free_all();

// /* testcase 14
// *create backstore whith iscsi_plugin error
// *
// *
// * @method input error iscsi_plugin number 
// *  
// * @result nothing established
// */
//     iscsi_del_all_targets();

//     /* initialize the storeobject */
//     sobj = (storage_obj_t*)malloc(sizeof(storage_obj_t));
//     strcpy(sobj->dev,"dev/sdb");
//     strcpy(sobj->name,"disk_b");
//     sobj->iscsi_plugin = 3;
//     sobj->size = 536870912;//512m

//     init_target();
//     init_portal();
//     init_para();
//     init_attr();

//     printf("Testcase14\n");
//     iscsi_create_target(sobj, target);

//     show_targetcli();
//     free_all();


// /* testcase 15
// *create backstore whith iscsi_plugin error
// *
// *
// * @method input  iscsi_plugin 0
// *  
// * @result nothing established
// */
//     iscsi_del_all_targets();

//     /* initialize the storeobject */
//     sobj = (storage_obj_t*)malloc(sizeof(storage_obj_t));
//     strcpy(sobj->dev,"dev/sdb");
//     strcpy(sobj->name,"disk_b");
//     sobj->iscsi_plugin = 0;
//     sobj->size = 536870912;//512m

//     init_target();
//     init_portal();
//     init_para();
//     init_attr();

//     printf("Testcase15\n");
//     iscsi_create_target(sobj, target);

//     show_targetcli();
//     free_all();



// /* testcase 16
// *create target whith target error
// * @default(fileio)
// *
// * @method input  target->wwn is empty
// * 
// * @result WWN not valid as: iqn, naa, eui
// * return 0 for create_target success : -1
// * nothing established
// */
//     iscsi_del_all_targets();

//     init_sobj();

//     target = (iscsitarget_t*)malloc(sizeof(iscsitarget_t));
//     strcpy(target->chap_mutual_password, "567899876556");
//     strcpy(target->chap_mutual_userid, "iqn.1991-05.com.microsoft:ibm-t410s");
//     strcpy(target->chap_password, "123455432112");
//     strcpy(target->chap_userid, "iqn.2003-01.org.linux-iscsi.localhost.x8664:sn.e16787b68bb4");
//     target->enable = 1;
//     target->tag = 1;
//     target->node_acls_num = 2;
//     strcpy((target->node_acls)[0],"iqn.1991-05.com.microsoft:ibm-t410s");
//     strcpy((target->node_acls)[1],"iqn.1994-05.com.redhat:a4bf13a296c");
//     strcpy(target->wwn, "");
 
//     init_portal();
//     init_para();
//     init_attr();

//     printf("Testcase16\n");
//     iscsi_create_target(sobj, target);

//     show_targetcli();
//     free_all();

// /* testcase 17
// *create target whith target error
// * @default(fileio)
// *
// * @method input  target->wwn  is aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
// *  
// * @result WWN not valid as: iqn, naa, eui
// * return 0 for create_target success : -1
// * nothing established
// */
//     iscsi_del_all_targets();

//     init_sobj();

//     target = (iscsitarget_t*)malloc(sizeof(iscsitarget_t));
//     strcpy(target->chap_mutual_password, "567899876556");
//     strcpy(target->chap_mutual_userid, "iqn.1991-05.com.microsoft:ibm-t410s");
//     strcpy(target->chap_password, "123455432112");
//     strcpy(target->chap_userid, "iqn.2003-01.org.linux-iscsi.localhost.x8664:sn.e16787b68bb4");
//     target->enable = 1;
//     target->tag = 1;
//     target->node_acls_num = 2;
//     strcpy((target->node_acls)[0],"iqn.1991-05.com.microsoft:ibm-t410s");
//     strcpy((target->node_acls)[1],"iqn.1994-05.com.redhat:a4bf13a296c");
//     strcpy(target->wwn, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");

//     init_portal();
//     init_para();
//     init_attr();

//     printf("Testcase17\n");
//     iscsi_create_target(sobj, target);

//     show_targetcli();
//     free_all();


// /* testcase 18
// *create tpg whith target error
// * @default(fileio)
// *
// * @method input  target->tag = 2;
// *  
// * @result nothing established
// */
//     iscsi_del_all_targets();

//     init_sobj();

//     target = (iscsitarget_t*)malloc(sizeof(iscsitarget_t));
//     strcpy(target->chap_mutual_password, "567899876556");
//     strcpy(target->chap_mutual_userid, "iqn.1991-05.com.microsoft:ibm-t410s");
//     strcpy(target->chap_password, "123455432112");
//     strcpy(target->chap_userid, "iqn.2003-01.org.linux-iscsi.localhost.x8664:sn.e16787b68bb4");
//     target->enable = 1;
//     target->tag = 2;
//     target->node_acls_num = 2;
//     strcpy((target->node_acls)[0],"iqn.1991-05.com.microsoft:ibm-t410s");
//     strcpy((target->node_acls)[1],"iqn.1994-05.com.redhat:a4bf13a296c");
//     strcpy(target->wwn, "iqn.2003-01.org.linux-iscsi.localhost.x8664:sn.e16787b68bb4");

//     init_portal();
//     init_para();
//     init_attr();

//     printf("Testcase18\n");
//     iscsi_create_target(sobj, target);

//     show_targetcli();
//     free_all();

// /* testcase 19
// *create tpg whith portals error
// * @default(fileio)
// *
// * @method input  target->portal->ip_address is "0.0.0.0"
// *  
// * @result no error
// */
//     iscsi_del_all_targets();

//     init_sobj();
//     init_target();

//     target->portal = (portals_t*)malloc(sizeof(portals_t));
//     strcpy(target->portal->ip_address, "0.0.0.0"); 
//     target->portal->iser = 1;
//     target->portal->port = 3260;

//     init_para();
//     init_attr();

//     printf("Testcase19\n");
//     iscsi_create_target(sobj, target);

//     show_targetcli();
//     free_all();

// /* testcase 20
// *create tpg whith portals error
// * @default(fileio)
// *
// * @method input  portal->ip_address "225.225.225.225"
// *  
// * @result no error
// */
//     iscsi_del_all_targets();

//     init_sobj();
//     init_target();

//     target->portal = (portals_t*)malloc(sizeof(portals_t));
//     strcpy(target->portal->ip_address, "225.225.225.225"); 
//     target->portal->iser = 1;
//     target->portal->port = 3260;

//     init_para();
//     init_attr();

//     printf("Testcase20\n");
//     iscsi_create_target(sobj, target);

//     show_targetcli();
//     free_all();


//  testcase 21
// *create tpg whith portals error
// * @default(fileio)
// *
// * @method input  portal->ip_address 0000
// *  
// *
// * @result no error

//     iscsi_del_all_targets();

//     init_sobj();
//     init_target();

//     target->portal = (portals_t*)malloc(sizeof(portals_t));
//     strcpy(target->portal->ip_address, "0000"); 
//     target->portal->iser = 1;
//     target->portal->port = 3260;

//     init_para();
//     init_attr();

//     printf("Testcase21\n");
//     iscsi_create_target(sobj, target);

//     show_targetcli();
//     free_all();

// /* testcase 22
// * create the portals 0.0.0.0:3260 in another target 
// *
// * @method set the portals 0.0.0.0  
// *
// *
// * @result could not create ip potals 0.0.0.0:3260 in another target
// */
//     iscsi_del_all_targets();
//     init_target();
//     init_sobj();
//     init_portal();
//     init_para();
//     init_attr();

//     printf("Testcase22\n");
//     iscsi_create_target(sobj, target);
//     show_targetcli();
//     free_all();


//     init_target();
//     init_sobj();
//     init_portal();
//     init_para();
//     init_attr();
//     strcpy(sobj->dev,"/dev/sdd");
//     strcpy(sobj->name,"disk_d");
//     strcpy(target->wwn, "iqn.2003-01.org.linux-iscsi.localhost.x8664:sn.e16787b68bb5");
//     strcpy(target->portal->ip_address, "0.0.0.0"); 
//     iscsi_create_target(sobj, target);



//     show_targetcli();
//     free_all();

// /* testcase 23
// * create the same portals in another target 
// *
// * @method give the same ip
// *
// *
// * @result  return success target with same ip portals
// * initiator could see 2 target
// */
//     iscsi_del_all_targets();
//     init_target();
//     init_sobj();
//     init_portal();
//     init_para();
//     init_attr();

//     printf("Testcase23\n");
//     iscsi_create_target(sobj, target);
//     show_targetcli();
//     free_all();


//     init_target();
//     init_sobj();
//     init_portal();
//     init_para();
//     init_attr();
//     strcpy(sobj->dev,"/dev/sdd");
//     strcpy(sobj->name,"disk_d");
//     strcpy(target->wwn, "iqn.2003-01.org.linux-iscsi.localhost.x8664:sn.e16787b68bb5");
//     strcpy(target->portal->ip_address, "192.168.1.77"); 
//     iscsi_create_target(sobj, target);



//     show_targetcli();
//     free_all();

// /* testcase 24
// * create the portals 0.0.0.0:3260 in first target and create another target of ip partals 192.168.1.77
// *
// * @method set the first portals 0.0.0.0  second target portal 192.168.1.77
// *
// *
// * @result could not create ip potals 192.168.1.77 in another target
// */
//     iscsi_del_all_targets();
//     init_target();
//     init_sobj();
//     init_portal();
//     init_para();
//     init_attr();
//     strcpy(sobj->dev,"/dev/sdd");
//     strcpy(sobj->name,"disk_d");
//     strcpy(target->wwn, "iqn.2003-01.org.linux-iscsi.localhost.x8664:sn.e16787b68bb5");
//     strcpy(target->portal->ip_address, "0.0.0.0"); 


//     printf("Testcase24\n");
//     iscsi_create_target(sobj, target);
//     show_targetcli();
//     free_all();


//     init_target();
//     init_sobj();
//     init_portal();
//     init_para();
//     init_attr();
//     iscsi_create_target(sobj, target);



//     show_targetcli();
//     free_all();


// /*testcase 25?
// *create tpg do not enable tpg
// * @default(fileio)
// *
// * @method input  target->enable =0
// *
// *
// * @result 
// */
//     iscsi_del_all_targets();
//     init_target();
//     init_sobj();
//     init_portal();
//     init_para();
//     init_attr();
//     target->enable =0;


//     printf("Testcas25\n");
//     iscsi_create_target(sobj, target);

//     show_targetcli();
//     free_all();

// /*testcase 26
// *create tpg with enable fault
// * @default(fileio)
// *
// * @method input  target->enable=ii
// *
// *
// * @result 
// */
//     iscsi_del_all_targets();
//     init_target();
//     init_sobj();
//     init_portal();
//     init_para();
//     init_attr();
//     target->enable =99;


//     printf("Testcas26\n");
//     iscsi_create_target(sobj, target);

//     show_targetcli();
//     free_all();

// /* testcase 27?
// *create target whith attribute error
// * @default(fileio)
// *
// * @method input  target->chap_mutual_password excced 16 bytes
// *  
// * @result nothing error
// */
//     iscsi_del_all_targets();

//     init_sobj();

//     target = (iscsitarget_t*)malloc(sizeof(iscsitarget_t));
//     strcpy(target->chap_mutual_password, "56789987655611111111111");
//     strcpy(target->chap_mutual_userid, "iqn.1991-05.com.microsoft:ibm-t410s");
//     strcpy(target->chap_password, "123455432112");
//     strcpy(target->chap_userid, "iqn.2003-01.org.linux-iscsi.localhost.x8664:sn.e16787b68bb4");
//     target->enable = 1;
//     target->tag = 1;
//     target->node_acls_num = 2;
//     strcpy((target->node_acls)[0],"iqn.1991-05.com.microsoft:ibm-t410s");
//     strcpy((target->node_acls)[1],"iqn.1994-05.com.redhat:a4bf13a296c");
//     strcpy(target->wwn, "iqn.2003-01.org.linux-iscsi.localhost.x8664:sn.e16787b68bb4");

//     init_portal();
//     init_para();
//     init_attr();

//     printf("Testcase27\n");
//     iscsi_create_target(sobj, target);

//     show_targetcli();
//     free_all();

// /* testcase 28?
// *create target whith attribute error
// * @default(fileio)
// *
// * @method input  target->chap_mutual_password less 12 bytes
// *  
// * @result nothing error
// */
//     iscsi_del_all_targets();

//     init_sobj();

//     target = (iscsitarget_t*)malloc(sizeof(iscsitarget_t));
//     strcpy(target->chap_mutual_password, "123");
//     strcpy(target->chap_mutual_userid, "iqn.1991-05.com.microsoft:ibm-t410s");
//     strcpy(target->chap_password, "123455432112");
//     strcpy(target->chap_userid, "iqn.2003-01.org.linux-iscsi.localhost.x8664:sn.e16787b68bb4");
//     target->enable = 1;
//     target->tag = 1;
//     target->node_acls_num = 2;
//     strcpy((target->node_acls)[0],"iqn.1991-05.com.microsoft:ibm-t410s");
//     strcpy((target->node_acls)[1],"iqn.1994-05.com.redhat:a4bf13a296c");
//     strcpy(target->wwn, "iqn.2003-01.org.linux-iscsi.localhost.x8664:sn.e16787b68bb4");

//     init_portal();
//     init_para();
//     init_attr();

//     printf("Testcase28\n");
//     iscsi_create_target(sobj, target);

//     show_targetcli();
//     free_all();

// /* testcase 29?
// *create target whith attribute error
// * @default(fileio)
// *
// * @method input  target->chap_mutual_userid not a proper name
// *  
// * @result nothing error
// */
//     iscsi_del_all_targets();

//     init_sobj();

//     target = (iscsitarget_t*)malloc(sizeof(iscsitarget_t));
//     strcpy(target->chap_mutual_password, "567899876556");
//     strcpy(target->chap_mutual_userid, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
//     strcpy(target->chap_password, "123455432112");
//     strcpy(target->chap_userid, "iqn.2003-01.org.linux-iscsi.localhost.x8664:sn.e16787b68bb4");
//     target->enable = 1;
//     target->tag = 1;
//     target->node_acls_num = 2;
//     strcpy((target->node_acls)[0],"iqn.1991-05.com.microsoft:ibm-t410s");
//     strcpy((target->node_acls)[1],"iqn.1994-05.com.redhat:a4bf13a296c");
//     strcpy(target->wwn, "iqn.2003-01.org.linux-iscsi.localhost.x8664:sn.e16787b68bb4");

//     init_portal();
//     init_para();
//     init_attr();

//     printf("Testcase29\n");
//     iscsi_create_target(sobj, target);

//     show_targetcli();
//     free_all();

// /* testcase 30
// *create target whith attribute error
// *
// *
// * @method input  target->node_acls_num  is bigger than the list
// *  
// * @result nothing established
// */
//     iscsi_del_all_targets();

//     init_sobj();

//     target = (iscsitarget_t*)malloc(sizeof(iscsitarget_t));
//     strcpy(target->chap_mutual_password, "567899876556");
//     strcpy(target->chap_mutual_userid, "iqn.1991-05.com.microsoft:ibm-t410s");
//     strcpy(target->chap_password, "123455432112");
//     strcpy(target->chap_userid, "iqn.2003-01.org.linux-iscsi.localhost.x8664:sn.e16787b68bb4");
//     target->enable = 1;
//     target->tag = 1;
//     target->node_acls_num = 4;
//     strcpy((target->node_acls)[0],"iqn.1991-05.com.microsoft:ibm-t410s");
//     strcpy((target->node_acls)[1],"iqn.1994-05.com.redhat:a4bf13a296c");
//     strcpy(target->wwn, "iqn.2003-01.org.linux-iscsi.localhost.x8664:sn.e16787b68bb4");

//     init_portal();
//     init_para();
//     init_attr();

//     printf("Testcase30\n");
//     iscsi_create_target(sobj, target);

//     show_targetcli();
//     free_all();

// /* testcase 31
// *create target whith attribute error
// * 
// *
// * @method input  target->node_acls_num  is smaller than the list
// *  
// * @result established the  number of  (target->node_acls_num) initiator
// */
//     iscsi_del_all_targets();

//     init_sobj();

//     target = (iscsitarget_t*)malloc(sizeof(iscsitarget_t));
//     strcpy(target->chap_mutual_password, "567899876556");
//     strcpy(target->chap_mutual_userid, "iqn.1991-05.com.microsoft:ibm-t410s");
//     strcpy(target->chap_password, "123455432112");
//     strcpy(target->chap_userid, "iqn.2003-01.org.linux-iscsi.localhost.x8664:sn.e16787b68bb4");
//     target->enable = 1;
//     target->tag = 1;
//     target->node_acls_num = 1;
//     strcpy((target->node_acls)[0],"iqn.1991-05.com.microsoft:ibm-t410s");
//     strcpy((target->node_acls)[1],"iqn.1994-05.com.redhat:a4bf13a296c");
//     strcpy(target->wwn, "iqn.2003-01.org.linux-iscsi.localhost.x8664:sn.e16787b68bb4");

//     init_portal();
//     init_para();
//     init_attr();

//     printf("Testcase31\n");
//     iscsi_create_target(sobj, target);

//     show_targetcli();
//     free_all();





// /* testcase 32
// *create target whith attribute error
// * 
// *
// * @method input  target->node_acls_num  is 0
// *  
// * @result target established success without acls
// */
//     iscsi_del_all_targets();

//     init_sobj();

//     target = (iscsitarget_t*)malloc(sizeof(iscsitarget_t));
//     strcpy(target->chap_mutual_password, "567899876556");
//     strcpy(target->chap_mutual_userid, "iqn.1991-05.com.microsoft:ibm-t410s");
//     strcpy(target->chap_password, "123455432112");
//     strcpy(target->chap_userid, "iqn.2003-01.org.linux-iscsi.localhost.x8664:sn.e16787b68bb4");
//     target->enable = 1;
//     target->tag = 1;
//     target->node_acls_num = 0;
//     strcpy((target->node_acls)[0],"iqn.1991-05.com.microsoft:ibm-t410s");
//     strcpy((target->node_acls)[1],"iqn.1994-05.com.redhat:a4bf13a296c");
//     strcpy(target->wwn, "iqn.2003-01.org.linux-iscsi.localhost.x8664:sn.e16787b68bb4");

//     init_portal();
//     init_para();
//     init_attr();

//     printf("Testcase32\n");
//     iscsi_create_target(sobj, target);

//     show_targetcli();
//     free_all();
    
// /* testcase 33
// *create target whith attribute error
// * 
// *
// * @method input  target->node_acls last one is empty
// *  
// * @result WWN not valid as: iqn, naa, eui
// * set_initiator failed
// * nothing established

// */
//     iscsi_del_all_targets();

//     init_sobj();

//     target = (iscsitarget_t*)malloc(sizeof(iscsitarget_t));
//     strcpy(target->chap_mutual_password, "567899876556");
//     strcpy(target->chap_mutual_userid, "iqn.1991-05.com.microsoft:ibm-t410s");
//     strcpy(target->chap_password, "123455432112");
//     strcpy(target->chap_userid, "iqn.2003-01.org.linux-iscsi.localhost.x8664:sn.e16787b68bb4");
//     target->enable = 1;
//     target->tag = 1;
//     target->node_acls_num = 2;
//     strcpy((target->node_acls)[0],"");
//     strcpy((target->node_acls)[1],"iqn.1994-05.com.redhat:a4bf13a296c");
//     strcpy(target->wwn, "iqn.2003-01.org.linux-iscsi.localhost.x8664:sn.e16787b68bb4");

//     init_portal();
//     init_para();
//     init_attr();

//     printf("Testcase33\n");
//     iscsi_create_target(sobj, target);

//     show_targetcli();
//     free_all();


// /* testcase 34
// *create target whith attribute error
// * 
// *
// * @method input  target->node_acls first one is empty
// *  
// * @result WWN not valid as: iqn, naa, eui
// * set_initiator failed
// * nothing established

// */
//     iscsi_del_all_targets();

//     init_sobj();

//     target = (iscsitarget_t*)malloc(sizeof(iscsitarget_t));
//     strcpy(target->chap_mutual_password, "567899876556");
//     strcpy(target->chap_mutual_userid, "iqn.1991-05.com.microsoft:ibm-t410s");
//     strcpy(target->chap_password, "123455432112");
//     strcpy(target->chap_userid, "iqn.2003-01.org.linux-iscsi.localhost.x8664:sn.e16787b68bb4");
//     target->enable = 1;
//     target->tag = 1;
//     target->node_acls_num = 2;
//     strcpy((target->node_acls)[0],"");
//     strcpy((target->node_acls)[1],"iqn.1994-05.com.redhat:a4bf13a296c");
//     strcpy(target->wwn, "iqn.2003-01.org.linux-iscsi.localhost.x8664:sn.e16787b68bb4");

//     init_portal();
//     init_para();
//     init_attr();

//     printf("Testcase34\n");
//     iscsi_create_target(sobj, target);

//     show_targetcli();
//     free_all();




// /*testcase 35 
// * set the ACLs mutual auth all empty 
// *
// * @method set the auth uncorrect 
// *
// *
// * @result  [1-way auth, Mapped LUNs: 1]
// */
//     iscsi_del_all_targets();

//     target = (iscsitarget_t*)malloc(sizeof(iscsitarget_t));
//     strcpy(target->chap_mutual_password, "");
//     strcpy(target->chap_mutual_userid, "");
//     strcpy(target->chap_password, "");
//     strcpy(target->chap_userid, "");
//     target->enable = 1;
//     target->tag = 1;
//     target->node_acls_num = 2;
//     strcpy((target->node_acls)[0],"iqn.1991-05.com.microsoft:ibm-t410s");
//     strcpy((target->node_acls)[1],"iqn.1994-05.com.redhat:a4bf13a296c");
//     strcpy(target->wwn, "iqn.2003-01.org.linux-iscsi.localhost.x8664:sn.e16787b68bb4");

//     init_sobj();
//     init_portal();
//     init_para();
//     init_attr();

//     printf("Testcase35\n");
//     iscsi_create_target(sobj, target);
//     iscsi_set_ACLs_auth(target->wwn, target->tag, target->node_acls, target->node_acls_num, target->chap_mutual_userid, target->chap_mutual_password, target->chap_userid, target->chap_password);

//     show_targetcli();
//     free_all();


// /* testcase 36
// * delete the target of wwn 
// *
// * @method give the wwn which do not exist  
// *
// *
// * @result return 0 for delete the target success : 1
// */
//     iscsi_del_all_targets();
//     init_target();
//     init_sobj();
//     init_portal();
//     init_para();
//     init_attr();

//     printf("Testcase36\n");
//     iscsi_create_target(sobj, target);
//     iscsi_del_target(true,"iqn.2003-01.org.linux-iscsi.localhost.x8664:sn.e16787b68bb5");

//     show_targetcli();
//     free_all();


// /* testcase 37
// *set acls initiator list
// * 
// *
// * @method input node_acls[i] one of them to 0
// *  
// * @result established the  target with the acls give
// */
//     iscsi_del_all_targets();

//     init_sobj();

//     target = (iscsitarget_t*)malloc(sizeof(iscsitarget_t));
//     strcpy(target->chap_mutual_password, "567899876556");
//     strcpy(target->chap_mutual_userid, "iqn.1991-05.com.microsoft:ibm-t410s");
//     strcpy(target->chap_password, "123455432112");
//     strcpy(target->chap_userid, "iqn.2003-01.org.linux-iscsi.localhost.x8664:sn.e16787b68bb4");
//     target->enable = 1;
//     target->tag = 1;
//     target->node_acls_num = 0;
//     strcpy((target->node_acls)[0],"iqn.1991-05.com.microsoft:ibm-t410s");
//     strcpy((target->node_acls)[1],"iqn.1994-05.com.redhat:a4bf13a296c");
//     strcpy(target->wwn, "iqn.2003-01.org.linux-iscsi.localhost.x8664:sn.e16787b68bb4");

//     init_portal();
//     init_para();
//     init_attr();

//     printf("Testcase37\n");
//     iscsi_create_target(sobj, target);

//     show_targetcli();
//     char wwn[] = "iqn.2003-01.org.linux-iscsi.localhost.x8664:sn.e16787b68bb4";
//     int tag = 1;
//     char node_acls[3][MAXNAMELEN];
//     strcpy((node_acls)[0],"iqn.1991-05.com.microsoft:ibm-t410s");
//    // strcpy((node_acls)[1],"iqn.1994-05.com.redhat:a4bf13a296c");
//     strcpy((node_acls)[2],"iqn.1994-05.com.redhat:a4bf13a296d");
//     int number = 3;
//     iscsi_set_initiators(wwn,tag,node_acls,number);
//     show_targetcli();
//     free_all();


//  testcase 38
// *set acls initiator list
// * 
// *
// * @method input number_38 smaller than the number of number_38
// *  
// * @result established the target with the number_38 of acls give

//     iscsi_del_all_targets();

//     init_sobj();

//     target = (iscsitarget_t*)malloc(sizeof(iscsitarget_t));
//     strcpy(target->chap_mutual_password, "567899876556");
//     strcpy(target->chap_mutual_userid, "iqn.1991-05.com.microsoft:ibm-t410s");
//     strcpy(target->chap_password, "123455432112");
//     strcpy(target->chap_userid, "iqn.2003-01.org.linux-iscsi.localhost.x8664:sn.e16787b68bb4");
//     target->enable = 1;
//     target->tag = 1;
//     target->node_acls_num = 0;
//     strcpy((target->node_acls)[0],"iqn.1991-05.com.microsoft:ibm-t410s");
//     strcpy((target->node_acls)[1],"iqn.1994-05.com.redhat:a4bf13a296c");
//     strcpy(target->wwn, "iqn.2003-01.org.linux-iscsi.localhost.x8664:sn.e16787b68bb4");

//     init_portal();
//     init_para();
//     init_attr();

//     printf("Testcase38\n");
//     iscsi_create_target(sobj, target);

//     show_targetcli();
//     char wwn_38[] = "iqn.2003-01.org.linux-iscsi.localhost.x8664:sn.e16787b68bb4";
//     int tag_38 = 1;
//     char node_acls_38[3][MAXNAMELEN];
//     strcpy(node_acls_38[0],"iqn.1991-05.com.microsoft:ibm-t410s");
//     strcpy(node_acls_38[1],"iqn.1994-05.com.redhat:a4bf13a296c");
//     strcpy(node_acls_38[2],"iqn.1994-05.com.redhat:a4bf13a296d");
//     int number_38 = 2;
//     iscsi_set_initiators(wwn_38,tag_38,node_acls_38,number_38);
//     show_targetcli();
//     free_all();

// /* testcase 39
// *set acls initiator list
// * 
// *
// * @method input number_39 bigger than the number of number_39
// *  
// * @result established the target with the number_39 of acls give
// */
//     iscsi_del_all_targets();

//     init_sobj();

//     target = (iscsitarget_t*)malloc(sizeof(iscsitarget_t));
//     strcpy(target->chap_mutual_password, "567899876556");
//     strcpy(target->chap_mutual_userid, "iqn.1991-05.com.microsoft:ibm-t410s");
//     strcpy(target->chap_password, "123455432112");
//     strcpy(target->chap_userid, "iqn.2003-01.org.linux-iscsi.localhost.x8664:sn.e16787b68bb4");
//     target->enable = 1;
//     target->tag = 1;
//     target->node_acls_num = 0;
//     strcpy((target->node_acls)[0],"iqn.1991-05.com.microsoft:ibm-t410s");
//     strcpy((target->node_acls)[1],"iqn.1994-05.com.redhat:a4bf13a296c");
//     strcpy(target->wwn, "iqn.2003-01.org.linux-iscsi.localhost.x8664:sn.e16787b68bb4");

//     init_portal();
//     init_para();
//     init_attr();

//     printf("Testcase39\n");
//     iscsi_create_target(sobj, target);

//     show_targetcli();
//     char wwn_39[] = "iqn.2003-01.org.linux-iscsi.localhost.x8664:sn.e16787b68bb4";
//     int tag_39 = 1;
//     char node_acls_39[3][MAXNAMELEN];
//     strcpy(node_acls_39[0],"iqn.1991-05.com.microsoft:ibm-t410s");
//     strcpy(node_acls_39[1],"iqn.1994-05.com.redhat:a4bf13a296c");
//     strcpy(node_acls_39[2],"iqn.1994-05.com.redhat:a4bf13a296d");
//     int number_39 = 5;
//     iscsi_set_initiators(wwn_39,tag_39,node_acls_39,number_39);
//     show_targetcli();
//     free_all();

// /* testcase 40
// *set and delete acls initiator list
// * 
// *
// * @method input number_40 under zero
// *  
// * @result give the hint number should greater than zero
// *
// */
//     iscsi_del_all_targets();

//     init_sobj();

//     target = (iscsitarget_t*)malloc(sizeof(iscsitarget_t));
//     strcpy(target->chap_mutual_password, "567899876556");
//     strcpy(target->chap_mutual_userid, "iqn.1991-05.com.microsoft:ibm-t410s");
//     strcpy(target->chap_password, "123455432112");
//     strcpy(target->chap_userid, "iqn.2003-01.org.linux-iscsi.localhost.x8664:sn.e16787b68bb4");
//     target->enable = 1;
//     target->tag = 1;
//     target->node_acls_num = 0;
//     strcpy((target->node_acls)[0],"iqn.1991-05.com.microsoft:ibm-t410s");
//     strcpy((target->node_acls)[1],"iqn.1994-05.com.redhat:a4bf13a296c");
//     strcpy(target->wwn, "iqn.2003-01.org.linux-iscsi.localhost.x8664:sn.e16787b68bb4");

//     init_portal();
//     init_para();
//     init_attr();

//     printf("Testcase40\n");
//     iscsi_create_target(sobj, target);
//     show_targetcli();



//     char wwn_40[] = "iqn.2003-01.org.linux-iscsi.localhost.x8664:sn.e16787b68bb4";
//     int tag_40 = 1;
//     char node_acls_40[3][MAXNAMELEN];
//     strcpy(node_acls_40[0],"iqn.1991-05.com.microsoft:ibm-t410s");
//     strcpy(node_acls_40[1],"iqn.1994-05.com.redhat:a4bf13a296c");
//     strcpy(node_acls_40[2],"iqn.1994-05.com.redhat:a4bf13a296d");
//     int number_40 = -1;
//     iscsi_set_initiators(wwn_40,tag_40,node_acls_40,number_40);
//     show_targetcli();

//     iscsi_del_initiators(wwn_40,tag_40,node_acls_40,number_40);
//     show_targetcli();


//     free_all();

// /* testcase 41
// *set and delete acls initiator list
// * 
// *
// * @method input number_41 greater than node_acls_number
// *  
// * @result running success delete all initiator
// *
// */
//     iscsi_del_all_targets();

//     init_sobj();

//     target = (iscsitarget_t*)malloc(sizeof(iscsitarget_t));
//     strcpy(target->chap_mutual_password, "567899876556");
//     strcpy(target->chap_mutual_userid, "iqn.1991-05.com.microsoft:ibm-t410s");
//     strcpy(target->chap_password, "123455432112");
//     strcpy(target->chap_userid, "iqn.2003-01.org.linux-iscsi.localhost.x8664:sn.e16787b68bb4");
//     target->enable = 1;
//     target->tag = 1;
//     target->node_acls_num = 0;
//     strcpy((target->node_acls)[0],"iqn.1991-05.com.microsoft:ibm-t410s");
//     strcpy((target->node_acls)[1],"iqn.1994-05.com.redhat:a4bf13a296c");
//     strcpy(target->wwn, "iqn.2003-01.org.linux-iscsi.localhost.x8664:sn.e16787b68bb4");

//     init_portal();
//     init_para();
//     init_attr();

//     printf("Testcase41\n");
//     iscsi_create_target(sobj, target);
//     show_targetcli();



//     char wwn_41[] = "iqn.2003-01.org.linux-iscsi.localhost.x8664:sn.e16787b68bb4";
//     int tag_41 = 1;
//     char node_acls_41[3][MAXNAMELEN];
//     strcpy(node_acls_41[0],"iqn.1991-05.com.microsoft:ibm-t410s");
//     strcpy(node_acls_41[1],"iqn.1994-05.com.redhat:a4bf13a296c");
//     strcpy(node_acls_41[2],"iqn.1994-05.com.redhat:a4bf13a296d");
//     int number_41 = 10;
//     iscsi_set_initiators(wwn_41,tag_41,node_acls_41,number_41);
//     show_targetcli();

//     iscsi_del_initiators(wwn_41,tag_41,node_acls_41,number_41);
//     show_targetcli();


//     free_all();

// /* testcase 42
// *set and delete acls initiator list
// * 
// *
// * @method input delete initiator number smaller than set initiator number
// *  
// * @result delete the first number of del_init_num_42 initialtor
// *
// */
//     iscsi_del_all_targets();

//     init_sobj();

//     target = (iscsitarget_t*)malloc(sizeof(iscsitarget_t));
//     strcpy(target->chap_mutual_password, "567899876556");
//     strcpy(target->chap_mutual_userid, "iqn.1991-05.com.microsoft:ibm-t410s");
//     strcpy(target->chap_password, "123455432112");
//     strcpy(target->chap_userid, "iqn.2003-01.org.linux-iscsi.localhost.x8664:sn.e16787b68bb4");
//     target->enable = 1;
//     target->tag = 1;
//     target->node_acls_num = 0;
//     strcpy((target->node_acls)[0],"iqn.1991-05.com.microsoft:ibm-t410s");
//     strcpy((target->node_acls)[1],"iqn.1994-05.com.redhat:a4bf13a296c");
//     strcpy(target->wwn, "iqn.2003-01.org.linux-iscsi.localhost.x8664:sn.e16787b68bb4");

//     init_portal();
//     init_para();
//     init_attr();

//     printf("Testcase42\n");
//     iscsi_create_target(sobj, target);
//     show_targetcli();



//     char wwn_42[] = "iqn.2003-01.org.linux-iscsi.localhost.x8664:sn.e16787b68bb4";
//     int tag_42 = 1;
//     char node_acls_42[3][MAXNAMELEN];
//     strcpy(node_acls_42[0],"iqn.1991-05.com.microsoft:ibm-t410s");
//     strcpy(node_acls_42[1],"iqn.1994-05.com.redhat:a4bf13a296c");
//     strcpy(node_acls_42[2],"iqn.1994-05.com.redhat:a4bf13a296d");
//     int number_42 = 4;
//     iscsi_set_initiators(wwn_42,tag_42,node_acls_42,number_42);
//     show_targetcli();
//     int del_init_num_42 = 2;
//     iscsi_del_initiators(wwn_42,tag_42,node_acls_42,del_init_num_42 );
//     show_targetcli();


//     free_all();

// /* testcase 43
// *set and delete acls initiator list
// * 
// *
// * @method input delete initiator number greater than set initiator number
// *  
// * @result delete all the  initialtor
// *
// */
//     iscsi_del_all_targets();

//     init_sobj();

//     target = (iscsitarget_t*)malloc(sizeof(iscsitarget_t));
//     strcpy(target->chap_mutual_password, "567899876556");
//     strcpy(target->chap_mutual_userid, "iqn.1991-05.com.microsoft:ibm-t410s");
//     strcpy(target->chap_password, "123455432112");
//     strcpy(target->chap_userid, "iqn.2003-01.org.linux-iscsi.localhost.x8664:sn.e16787b68bb4");
//     target->enable = 1;
//     target->tag = 1;
//     target->node_acls_num = 0;
//     strcpy((target->node_acls)[0],"iqn.1991-05.com.microsoft:ibm-t410s");
//     strcpy((target->node_acls)[1],"iqn.1994-05.com.redhat:a4bf13a296c");
//     strcpy(target->wwn, "iqn.2003-01.org.linux-iscsi.localhost.x8664:sn.e16787b68bb4");

//     init_portal();
//     init_para();
//     init_attr();

//     printf("Testcase43\n");
//     iscsi_create_target(sobj, target);
//     show_targetcli();



//     char wwn_43[] = "iqn.2003-01.org.linux-iscsi.localhost.x8664:sn.e16787b68bb4";
//     int tag_43 = 1;
//     char node_acls_43[3][MAXNAMELEN];
//     strcpy(node_acls_43[0],"iqn.1991-05.com.microsoft:ibm-t410s");
//     strcpy(node_acls_43[1],"iqn.1994-05.com.redhat:a4bf13a296c");
//     strcpy(node_acls_43[2],"iqn.1994-05.com.redhat:a4bf13a296d");
//     int number_43 = 4;
//     iscsi_set_initiators(wwn_43,tag_43,node_acls_43,number_43);
//     show_targetcli();
//     int del_init_num_43 = 10;
//     iscsi_del_initiators(wwn_43,tag_43,node_acls_43,del_init_num_43 );
//     show_targetcli();


//     free_all();

    return 0;
}