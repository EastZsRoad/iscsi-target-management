#include <stdio.h>
#include <stdlib.h>
#include <python2.7/Python.h>
#include <stdbool.h>
#include <string.h>

#include "iscsi_manage.h"

#define MAXLUNS 10
#define MAXNAMELEN 100



parameters_t *parame;
attributes_t *attrib;
iscsitarget_t *target;
storage_obj_t *sobj;
portals_t *portal;




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
    sobj->plugin = 2;
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
    target->parame->AuthMethod = 2;
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

/****************************Success Test*******************************/

/*testcase 1
* create target success
*
*/
    iscsi_del_all_targets();
    init_target();
    init_sobj();
    init_portal();
    init_para();
    init_attr();

    printf("Testcase1\n");
    iscsi_create_target(sobj, target);

    show_targetcli();
    free_all();


// /*testcase 2
// * set ACLs mutual auth 
// *
// */
//     iscsi_del_all_targets();
//     init_target();
//     init_sobj();
//     init_portal();
//     init_para();
//     init_attr();

//     printf("Testcase2\n");
//     iscsi_create_target(sobj, target);
//     iscsi_set_ACLs_auth(target->wwn, target->tag, target->node_acls, target->chap_mutual_userid, target->chap_mutual_password, target->chap_userid, target->chap_password, target->node_acls_num);
    
//     show_targetcli();
//     free_all();

// /*testcase 3 
// * delete the ACLs mutual auth  
// *
// *
// *
// */
//     iscsi_del_all_targets();
//     init_target();
//     init_sobj();
//     init_portal();
//     init_para();
//     init_attr();

//     printf("Testcase3\n");
//     iscsi_create_target(sobj, target);
//     iscsi_set_ACLs_auth(target->wwn, target->tag, target->node_acls, target->chap_mutual_userid, target->chap_mutual_password, target->chap_userid, target->chap_password, target->node_acls_num);
//     iscsi_del_ACLs_auth(target->wwn, target->tag);

//     show_targetcli();
//     free_all();


// /* testcase 4 
// * delete the target of wwn 
// *
// */
//     iscsi_del_all_targets();
//     init_target();
//     init_sobj();
//     init_portal();
//     init_para();
//     init_attr();

//     printf("Testcase4\n");
//     iscsi_create_target(sobj, target);
//     iscsi_del_target(target->wwn);

//     show_targetcli();
//     free_all();

// /* testcase 5 
// *create fileio target
// *
// * @parameter change
// *   sobj = (storage_obj_t*)malloc(sizeof(storage_obj_t));
// *   strcpy(sobj->dev,"/root/Desktop/LibreOffice_4.4.4_Win_x86.msi");//214 MB(224,423,936)
// *   strcpy(sobj->name,"LibreOffice_4");
// *   sobj->plugin = 1;
// *   sobj->size = 12345;
// *
// */  iscsi_del_all_targets();

//     /* initialize the storeobject */
//     sobj = (storage_obj_t*)malloc(sizeof(storage_obj_t));
//     strcpy(sobj->dev,"/root/Desktop/LibreOffice_4.4.4_Win_x86.msi");
//     strcpy(sobj->name,"LibreOffice_4");
//     sobj->plugin = 1;
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

// /* testcase 6 -1
// *create target whith file error
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
//     sobj->plugin = 1;
//     sobj->size = 53687091200;

//     init_target();
//     init_portal();
//     init_para();
//     init_attr();

//     printf("Testcase6 -1\n");
//     iscsi_create_target(sobj, target);

//     show_targetcli();
//     free_all();

// /* testcase 6 -2
// *create target whith file error
// *
// *
// * @method input fileiosize is 0
// *  
// *
// * @result return 0 size file
// */
//     iscsi_del_all_targets();

//     /* initialize the storeobject */
//     sobj = (storage_obj_t*)malloc(sizeof(storage_obj_t));
//     strcpy(sobj->dev,"/root/Desktop/LibreOffice_4.4.4_Win_x86.msi");//214m(224,423,936)
//     strcpy(sobj->name,"LibreOffice_4");
//     sobj->plugin = 1;
//     sobj->size = 0;//

//     init_target();
//     init_portal();
//     init_para();
//     init_attr();

//     printf("Testcase6 -2\n");
//     iscsi_create_target(sobj, target);

//     show_targetcli();
//     free_all();

// /* testcase 6 -3
// *create target whith file error
// *
// *
// * @method do not give the fileiosize 
// *  
// *
// * @result  return(127.1TiB)!!?
// */
//     iscsi_del_all_targets();

//     /* initialize the storeobject */
//     sobj = (storage_obj_t*)malloc(sizeof(storage_obj_t));
//     strcpy(sobj->dev,"/root/Desktop/LibreOffice_4.4.4_Win_x86.msi");//214m(224,423,936)
//     strcpy(sobj->name,"LibreOffice_4");
//     sobj->plugin = 1;
//     //sobj->size = 0;//

//     init_target();
//     init_portal();
//     init_para();
//     init_attr();

//     printf("Testcase6 -3\n");
//     iscsi_create_target(sobj, target);

//     show_targetcli();
//     free_all();

// /* testcase 6 -4
// *create target whith file error
// *
// *
// * @method do not give the fileiosize 
// *  
// *
// * @result  return(127.1TiB) also!!?
// */
//     iscsi_del_all_targets();

//     /* initialize the storeobject */
//     sobj = (storage_obj_t*)malloc(sizeof(storage_obj_t));
//     strcpy(sobj->dev,"/root/Desktop/port_set.tar");//214m(224,423,936)
//     strcpy(sobj->name,"port_set");
//     sobj->plugin = 1;
//     //sobj->size = 0;//

//     init_target();
//     init_portal();
//     init_para();
//     init_attr();

//     printf("Testcase6 -4\n");
//     iscsi_create_target(sobj, target);

//     show_targetcli();
//     free_all();

// /* testcase 7-1
// *create target whith file error
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
//     sobj->plugin = 1;
//     //sobj->size = 0;//

//     init_target();
//     init_portal();
//     init_para();
//     init_attr();

//     printf("Testcase7 -1\n");
//     iscsi_create_target(sobj, target);

//     show_targetcli();
//     free_all();
    
// /* testcase 7-2
// *create target whith file error
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
//     strcpy(sobj->name,"LibreOffice_4");
//     sobj->plugin = 1;
//     sobj->size = 536870912;//

//     init_target();
//     init_portal();
//     init_para();
//     init_attr();

//     printf("Testcase7 -2\n");
//     iscsi_create_target(sobj, target);

//     show_targetcli();
//     free_all();

// /* testcase 7-3
// *create target whith file error
// *
// *
// * @method input file but the plugin is block
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
//     sobj->plugin = 2;
//     sobj->size = 536870912;//

//     init_target();
//     init_portal();
//     init_para();
//     init_attr();

//     printf("Testcase7 -3\n");
//     iscsi_create_target(sobj, target);

//     show_targetcli();
//     free_all();


// /* testcase 8
// *create target whith file error
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
//     sobj->plugin = 1;
//     sobj->size = 536870912;//512m

//     init_target();
//     init_portal();
//     init_para();
//     init_attr();

//     printf("Testcase8\n");
//     iscsi_create_target(sobj, target);

//     show_targetcli();
//     free_all();


// /* testcase 9
// *create target whith block error
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
//     sobj->plugin = 2;
//     sobj->size = 536870912;//512m

//     init_target();
//     init_portal();
//     init_para();
//     init_attr();

//     printf("Testcase9\n");
//     iscsi_create_target(sobj, target);

//     show_targetcli();
//     free_all();

// /* testcase 10
// *create target whith plugin error
// *
// *
// * @method input error plugin number 
// *  
// * @result nothing established
// */
//     iscsi_del_all_targets();

//     /* initialize the storeobject */
//     sobj = (storage_obj_t*)malloc(sizeof(storage_obj_t));
//     strcpy(sobj->dev,"dev/sdb");
//     strcpy(sobj->name,"disk_b");
//     sobj->plugin = 3;
//     sobj->size = 536870912;//512m

//     init_target();
//     init_portal();
//     init_para();
//     init_attr();

//     printf("Testcase10\n");
//     iscsi_create_target(sobj, target);

//     show_targetcli();
//     free_all();


// /* testcase 11
// *create target whith plugin error
// *
// *
// * @method input  plugin 0
// *  
// * @result nothing established
// */
//     iscsi_del_all_targets();

//     /* initialize the storeobject */
//     sobj = (storage_obj_t*)malloc(sizeof(storage_obj_t));
//     strcpy(sobj->dev,"dev/sdb");
//     strcpy(sobj->name,"disk_b");
//     sobj->plugin = 0;
//     sobj->size = 536870912;//512m

//     init_target();
//     init_portal();
//     init_para();
//     init_attr();

//     printf("Testcase11\n");
//     iscsi_create_target(sobj, target);

//     show_targetcli();
//     free_all();



// /* testcase 12
// *create target whith target error
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

//     printf("Testcase12\n");
//     iscsi_create_target(sobj, target);

//     show_targetcli();
//     free_all();

// /* testcase 13
// *create target whith target error
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

//     printf("Testcase13\n");
//     iscsi_create_target(sobj, target);

//     show_targetcli();
//     free_all();

// /* testcase 14
// *create target whith target error
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

//     printf("Testcase14\n");
//     iscsi_create_target(sobj, target);

//     show_targetcli();
//     free_all();

// /* testcase 15
// *create target whith target error
// * @default(fileio)
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

//     printf("Testcase15\n");
//     iscsi_create_target(sobj, target);

//     show_targetcli();
//     free_all();

// /* testcase 16
// *create target whith target error
// * @default(fileio)
// *
// * @method input  target->node_acls_num  is smaller than the list
// *  
// * @result established the  first (target->node_acls_num) initiator
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

//     printf("Testcase16\n");
//     iscsi_create_target(sobj, target);

//     show_targetcli();
//     free_all();





// /* testcase 17
// *create target whith target error
// * @default(fileio)
// *
// * @method input  target->node_acls_num  is 0
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
//     target->node_acls_num = 0;
//     strcpy((target->node_acls)[0],"iqn.1991-05.com.microsoft:ibm-t410s");
//     strcpy((target->node_acls)[1],"iqn.1994-05.com.redhat:a4bf13a296c");
//     strcpy(target->wwn, "iqn.2003-01.org.linux-iscsi.localhost.x8664:sn.e16787b68bb4");

//     init_portal();
//     init_para();
//     init_attr();

//     printf("Testcase17\n");
//     iscsi_create_target(sobj, target);

//     show_targetcli();
//     free_all();
    
// /* testcase 18
// *create target whith target error
// * @default(fileio)
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
//     strcpy((target->node_acls)[0],"iqn.1991-05.com.microsoft:ibm-t410s");
//     strcpy((target->node_acls)[1],"");
//     strcpy(target->wwn, "iqn.2003-01.org.linux-iscsi.localhost.x8664:sn.e16787b68bb4");

//     init_portal();
//     init_para();
//     init_attr();

//     printf("Testcase18\n");
//     iscsi_create_target(sobj, target);

//     show_targetcli();
//     free_all();

// /* testcase 19
// *create target whith target error
// * @default(fileio)
// *
// * @method input  target->wwn is 0
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

//     printf("Testcase19\n");
//     iscsi_create_target(sobj, target);

//     show_targetcli();
//     free_all();


// /* testcase 20
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

//     printf("Testcase20\n");
//     iscsi_create_target(sobj, target);

//     show_targetcli();
//     free_all();


// /* testcase 21
// *create target whith target error
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

//     printf("Testcase21\n");
//     iscsi_create_target(sobj, target);

//     show_targetcli();
//     free_all();


// /* testcase 22
// *create target whith portals error
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

//     printf("Testcase22\n");
//     iscsi_create_target(sobj, target);

//     show_targetcli();
//     free_all();

// /* testcase 23
// *create target whith portals error
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

//     printf("Testcase23\n");
//     iscsi_create_target(sobj, target);

//     show_targetcli();
//     free_all();


// /* testcase 24
// *create target whith portals error
// * @default(fileio)
// *
// * @method input  portal->ip_address 0000
// *  
// *
// * @result no error
// */
//     iscsi_del_all_targets();

//     init_sobj();
//     init_target();

//     target->portal = (portals_t*)malloc(sizeof(portals_t));
//     strcpy(target->portal->ip_address, "0000"); 
//     target->portal->iser = 1;
//     target->portal->port = 3260;

//     init_para();
//     init_attr();

//     printf("Testcase24\n");
//     iscsi_create_target(sobj, target);

//     show_targetcli();
//     free_all();

// /*testcase 25
// *create target whith portals error
// * @default(fileio)
// *
// * @method input  portal->ip_address 0000
// *
// *
// * @result no error
// */
//     iscsi_del_all_targets();
//     init_target();
//     init_sobj();
//     init_portal();
//     init_para();

//     target->attrib = (attributes_t*)malloc(sizeof(attributes_t));
//     target->attrib->authentication = 0;
//     target->attrib->cache_dynamic_acls = 1;
//     target->attrib->default_cmdsn_depth = 16;
//     target->attrib->demo_mode_write_protect = 0;
//     target->attrib->generate_node_acls = 1;
//     target->attrib->login_timeout = 15;
//     target->attrib->netif_timeout = 2;
//     target->attrib->prod_mode_write_protect = 0;

//     printf("Testcase25\n");
//     iscsi_create_target(sobj, target);

//     show_targetcli();
//     free_all();

// /*testcase 26 
// * set the ACLs mutual auth  
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

//     printf("Testcase26\n");
//     iscsi_create_target(sobj, target);
//     iscsi_set_ACLs_auth(target->wwn, target->tag, target->node_acls, target->chap_mutual_userid, target->chap_mutual_password, target->chap_userid, target->chap_password, target->node_acls_num);

//     show_targetcli();
//     free_all();


// /* testcase 27 
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

//     printf("Testcase27\n");
//     iscsi_create_target(sobj, target);
//     iscsi_del_target("iqn.2003-01.org.linux-iscsi.localhost.x8664:sn.e16787b68bb5");

//     show_targetcli();
//     free_all();









    return 0;
}