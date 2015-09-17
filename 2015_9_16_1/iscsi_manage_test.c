#include <stdio.h>
#include <stdlib.h>
#include <python2.7/Python.h>
#include <stdbool.h>
#include <string.h>

#include "iscsi_manage.h"

int main(int argc, char* argv[])
{


    iscsi_del_all_targets();


// /*
// *testcase 0
// * create sucess
// *
// */
//     /* initialize the target */
//     iscsitarget_t *target;
//     target = (iscsitarget_t*)malloc(sizeof(iscsitarget_t));
//     strcpy(target->chap_mutual_password, "567899876556");
//     strcpy(target->chap_mutual_userid, "iqn.1991-05.com.microsoft:ibm-t410s");
//     strcpy(target->chap_password, "123455432112");
//     strcpy(target->chap_userid, "iqn.2003-01.org.linux-iscsi.localhost.x8664:sn.e16787b68bb4");
//     target->enable = 1;
//     target->tag = 1;

//     /* initialize the storeobject */
//     storage_obj_t *sobj;
//     sobj = (storage_obj_t*)malloc(sizeof(storage_obj_t));
//     strcpy(sobj->dev,"/dev/sdc");
//     strcpy(sobj->name,"disk_c");
//     sobj->plugin = 2;
//     sobj->size = 512000000;

//     /* initialize the portals */
//     //portals_t *portal;
//     target->portal = (portals_t*)malloc(sizeof(portals_t));
//     strcpy(target->portal->ip_address, "192.168.1.77"); 
//     target->portal->iser = 1;
//     target->portal->port = 3260;

//     /* initialize the parameters */
//     //parameters_t *parame;
//     target->parame = (parameters_t*)malloc(sizeof(parameters_t));
//     target->parame->AuthMethod = 2;

//     /* initialize the attributes */
//    // attributes_t *attrib;
//     target->attrib = (attributes_t*)malloc(sizeof(attributes_t));
//     target->attrib->authentication = 0;
//     target->attrib->cache_dynamic_acls = 1;
//     target->attrib->default_cmdsn_depth = 16;
//     target->attrib->demo_mode_write_protect = 0;
//     target->attrib->generate_node_acls = 1;
//     target->attrib->login_timeout = 15;
//     target->attrib->netif_timeout = 2;
//     target->attrib->prod_mode_write_protect = 0;
//     printf("testcase0\n");
//     creat_fileio_target(sobj, target);

//     /* this block is create for testcase 6 */
//     char auth_wwn[MAXNAMELEN];
//     char auth_mutual_user[MAXCHAPUSER];
//     char auth_mutual_passwd[MAXCHAPPWD];
//     char auth_user[MAXCHAPUSER];
//     char auth_passwd[MAXCHAPPWD];
//     strcpy(auth_wwn, sobj->wwn); 
//     strcpy(auth_mutual_passwd, target->chap_mutual_password);
//     strcpy(auth_mutual_user, target->chap_mutual_userid);
//     strcpy(auth_passwd, target->chap_password);
//     strcpy(auth_user, target->chap_userid);



//     /* free the memory */
//     free(target);
//     free(sobj);






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
    strcpy(sobj->dev,"/dev/sdf");
    strcpy(sobj->name,"disk_f");
    sobj->plugin = 2;
    sobj->size = 2147483648;

    /* initialize the portals */
    //portals_t *portal;
    target->portal = (portals_t*)malloc(sizeof(portals_t));
    strcpy(target->portal->ip_address, "192.168.1.77"); 
    target->portal->iser = 1;
    target->portal->port = 3260;

    /* initialize the parameters */
    //parameters_t *parame;
    target->parame = (parameters_t*)malloc(sizeof(parameters_t));
    target->parame->AuthMethod = 2;

    /* initialize the attributes */
   // attributes_t *attrib;
    target->attrib = (attributes_t*)malloc(sizeof(attributes_t));
    target->attrib->authentication = 0;
    target->attrib->cache_dynamic_acls = 1;
    target->attrib->default_cmdsn_depth = 16;
    target->attrib->demo_mode_write_protect = 0;
    target->attrib->generate_node_acls = 1;
    target->attrib->login_timeout = 15;
    target->attrib->netif_timeout = 2;
    target->attrib->prod_mode_write_protect = 0;
    printf("testcase1\n");
    iscsi_create_block_target(sobj, target);

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
    iscsi_set_initiators(wwn_acl, tag, node_acls, number);



}
//delete
//creat_block_target