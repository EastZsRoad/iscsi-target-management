#include <stdio.h>
#include <stdlib.h>
#include <python2.7/Python.h>
#include <stdbool.h>
#include <string.h>

#include "iscsi_manage.h"


#define RTSLIB_WRAPPER  "iscsi_rtslib_wrapper"


static int iscsi_del_backstore(int plugin, char name[]);

 
#define __DEBUG__ 
#ifdef __DEBUG__
#define DEBUG(format,...) printf("File: "__FILE__", Line: %d: "format, __LINE__, ##__VA_ARGS__)
#else
#define DEBUG(format,...)
#endif


/*
 *  create and enable an iscsi  target
 *
 *  execute sequence:
 *  1)create_backstore
 *  2)creat_target
 *  3)create_tpg
 *  4)map_target2sobj
 *  5)enable_tpg
 *  6)set_tpg_attr
 *
 *  @sobj   storage obj for the target; create the obj if it doesn't exist
 *  @target iscsi target definition
 *
 *  @return wwn of the storage obj in target->wwn if was created
 *          0 success, -1 otherwise.
 */
int iscsi_create_target(storage_obj_t *sobj, iscsitarget_t *target)
{

    PyObject *pName, *pModule, *pDict, *pFunc, *pArgs, *pRetVal;

    int result;

    int step_1 = -1, step_2 = -1, step_3 = -1, step_4 = -1,step_5 = -1,step_6 = -1, step_7 = -1;
    
    Py_Initialize();
    if(!Py_IsInitialized())   
    {  
        printf("cannot find rts");
        getchar();
        return -1;  
    }

    PyRun_SimpleString("import sys");
    PyRun_SimpleString("sys.path.append('./')");

    pName = PyString_FromString(RTSLIB_WRAPPER); 
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

    /*create bacstore*/
    pFunc = PyDict_GetItemString(pDict,"create_backstore");
    if ( !pFunc || !PyCallable_Check(pFunc) )  
    {
        DEBUG("can't find create_backstore");  
        getchar();  
        return -1;  
    }
    pArgs = PyTuple_New(4);
    PyTuple_SetItem(pArgs, 0, Py_BuildValue("i",sobj->plugin));
    PyTuple_SetItem(pArgs, 1, Py_BuildValue("s",sobj->name));
    PyTuple_SetItem(pArgs, 2, Py_BuildValue("s",sobj->dev));
    PyTuple_SetItem(pArgs, 3, Py_BuildValue("l",sobj->size));
    pRetVal = PyObject_CallObject(pFunc, pArgs);
    step_1 = PyInt_AsLong(pRetVal);
    result = step_1;
    Py_DECREF(pArgs);
    Py_DECREF(pRetVal);

    DEBUG("return 0 for create_backstore success : %d\n", step_1);

    if (step_1 == 0)
    {
        /*create target*/
        pFunc = PyDict_GetItemString(pDict,"create_target");
        if ( !pFunc || !PyCallable_Check(pFunc) )  
        {
            printf("can't find create_target");  
            getchar();  
            return -1;  
        }
        pArgs = PyTuple_New(1);
        PyTuple_SetItem(pArgs, 0, Py_BuildValue("s",target->wwn));
        pRetVal = PyObject_CallObject(pFunc,pArgs);
        step_2 = PyInt_AsLong(pRetVal);
        result = step_2;
        Py_DECREF(pArgs);
        Py_DECREF(pRetVal);
        DEBUG("return 0 for create_target success : %d\n", step_2);
        if (step_2 == 0)
        {
            /*creat tpg*/
            pFunc = PyDict_GetItemString(pDict,"create_tpg");
            if ( !pFunc || !PyCallable_Check(pFunc) )  
            {
                printf("can't find create_tpg");  
                getchar();  
                return -1;  
            }
            pArgs = PyTuple_New(3);
            PyTuple_SetItem(pArgs, 0, Py_BuildValue("s",target->wwn));
            PyTuple_SetItem(pArgs, 1, Py_BuildValue("s",target->portal->ip_address));
            PyTuple_SetItem(pArgs, 2, Py_BuildValue("i",target->portal->port));
            pRetVal = PyObject_CallObject(pFunc, pArgs);
            step_3 = PyInt_AsLong(pRetVal);
            Py_DECREF(pArgs);
            Py_DECREF(pRetVal);
            DEBUG("return 0 for create_tpg success : %d\n", step_3);


             /*map_target2sobj*/
            pFunc = PyDict_GetItemString(pDict,"map_target2sobj");
            if ( !pFunc || !PyCallable_Check(pFunc) )  
            {
                printf("can't find map_target2sobj");  
                getchar();  
                return -1;  
            }
            pArgs = PyTuple_New(4);
            PyTuple_SetItem(pArgs, 0, Py_BuildValue("s",target->wwn));
            PyTuple_SetItem(pArgs, 1, Py_BuildValue("i",target->tag));
            PyTuple_SetItem(pArgs, 2, Py_BuildValue("i",sobj->plugin));
            PyTuple_SetItem(pArgs, 3, Py_BuildValue("s",sobj->name));
            pRetVal = PyObject_CallObject(pFunc, pArgs);
            step_4 = PyInt_AsLong(pRetVal);
            Py_DECREF(pArgs);
            Py_DECREF(pRetVal);
            DEBUG("return 0 for map success : %d\n",step_4);
            
            

            /*enable the tpg*/
            pFunc = PyDict_GetItemString(pDict,"enable_tpg");
            if ( !pFunc || !PyCallable_Check(pFunc) )  
            {
                printf("can't find block enable_tpg");  
                getchar();  
                return -1;  
            }
            pArgs = PyTuple_New(3);
            PyTuple_SetItem(pArgs, 0, Py_BuildValue("s",target->wwn));
            PyTuple_SetItem(pArgs, 1, Py_BuildValue("i",target->tag));
            PyTuple_SetItem(pArgs, 2, Py_BuildValue("i",target->enable));
            pRetVal = PyObject_CallObject(pFunc, pArgs);
            step_5 = PyInt_AsLong(pRetVal);
            Py_DECREF(pArgs);
            Py_DECREF(pRetVal);
            DEBUG("return 0 for enable_tpg success : %d\n", step_5);
            

            /*set the attribute of the tpg*/
            pFunc = PyDict_GetItemString(pDict,"set_tpg_attr");
            if ( !pFunc || !PyCallable_Check(pFunc) )  
            {
                printf("can't find block enable_tpg");  
                getchar();  
                return -1;  
            }
            pArgs = PyTuple_New(10);
            PyTuple_SetItem(pArgs, 0, Py_BuildValue("s",target->wwn));
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
            step_6 = PyInt_AsLong(pRetVal);
            Py_DECREF(pArgs);
            Py_DECREF(pRetVal);
            DEBUG("return 0 for set_tpg_attr sucess : %d\n", step_6);
            
            /*set initiatorlist*/
            pFunc = PyDict_GetItemString(pDict,"set_initiator");
            if ( !pFunc || !PyCallable_Check(pFunc) )  
            {
                printf("can't find block set_initiator");  
                getchar();  
                return -1;  
            }
            int i ;
            for (i = 0; i < target->node_acls_num; ++i)
            {
                pArgs = PyTuple_New(3);
                PyTuple_SetItem(pArgs, 0, Py_BuildValue("s",target->wwn));
                PyTuple_SetItem(pArgs, 1, Py_BuildValue("i",target->tag));
                PyTuple_SetItem(pArgs, 2, Py_BuildValue("s",(target->node_acls)[i]));
                pRetVal = PyObject_CallObject(pFunc, pArgs);

                if (PyInt_AsLong(pRetVal) == 0)
                {
                     DEBUG ("set_initiator success\n");
                     step_7 = 0;
                }else if (PyInt_AsLong(pRetVal) == -1)
                {
                    printf ("set_initiator failed\n");
                    // iscsi_del_initiators(target->wwn, target->tag, target->node_acls, i);
                    step_7 = -1;
                }
                Py_DECREF(pArgs);
                Py_DECREF(pRetVal);

            }

            int result = (step_2||step_3||step_4||step_5||step_6||step_7);
            if (result ==0)
            {
                printf("creat_block_target %s success\n",target->wwn);
            
            }
            else
            {
                printf("create target failed so delete the backstore and target\n");
                iscsi_del_backstore(sobj->plugin, sobj->name);
                iscsi_del_target(target->wwn);
            }
        }
        else
        {
            printf("creat backstore success but create target failed\n");
            iscsi_del_backstore(sobj->plugin, sobj->name);


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
    Py_DECREF(pModule);

    Py_Finalize();

    return result;
}



/*
 * remove all targets
 *
 * @return 0 success; -1 otherwise
 */

int iscsi_del_all_targets(void)
{
    PyObject *pName, *pModule, *pDict, *pFunc, *pRetVal;
    int result;  
    
    Py_Initialize();
    if(!Py_IsInitialized())   
    {  
        return -1;  
    }

    PyRun_SimpleString("import sys");
    PyRun_SimpleString("sys.path.append('./')");

    pName = PyString_FromString(RTSLIB_WRAPPER); 
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
    result = PyInt_AsLong(pRetVal);
    DEBUG("return 0 for delete success : %d\n", result); 


    Py_DECREF(pName);
    Py_DECREF(pModule);
    Py_DECREF(pRetVal);
    Py_Finalize();

    return result;
}


/*
 * remove an iscsi target
 *
 * @wwn iscsi target's wwn
 *
 * @return 0 success; -1 otherwise
 */
int iscsi_del_target(char wwn[])
{
    PyObject *pName, *pModule, *pDict, *pFunc, *pArgs, *pRetVal;
    int result;
    
    Py_Initialize();
    if(!Py_IsInitialized())   
    {  
        printf("cannot find rts");
        getchar();
        return -1;  
    }

    PyRun_SimpleString("import sys");
    PyRun_SimpleString("sys.path.append('./')");

    pName = PyString_FromString(RTSLIB_WRAPPER); 
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
    result = PyInt_AsLong(pRetVal);
    DEBUG("return 0 for delete the target success : %d\n", result);

    Py_DECREF(pName);
    Py_DECREF(pArgs);
    Py_DECREF(pModule);
    Py_DECREF(pRetVal);

    Py_Finalize();
    return result;
}

/*
 * remove an iscsi backstore
 *
 * @wwn iscsi backstore's name
 *
 * @return 0 success; -1 otherwise
 */
static int iscsi_del_backstore(int plugin, char name[])
{
    PyObject *pName, *pModule, *pDict, *pFunc, *pArgs, *pRetVal;
    int result;
    
    Py_Initialize();
    if(!Py_IsInitialized())   
    {  
        printf("cannot find rts");
        getchar();
        return -1;  
    }

    PyRun_SimpleString("import sys");
    PyRun_SimpleString("sys.path.append('./')");

    pName = PyString_FromString(RTSLIB_WRAPPER); 
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
    pArgs = PyTuple_New(2);
    PyTuple_SetItem(pArgs, 0, Py_BuildValue("i",plugin));
    PyTuple_SetItem(pArgs, 1, Py_BuildValue("s",name));
    pRetVal = PyObject_CallObject(pFunc, pArgs);
    result = PyInt_AsLong(pRetVal);
    DEBUG("return 0 for delete the delete_backstore success : %d\n", result);

    Py_DECREF(pName);
    Py_DECREF(pArgs);
    Py_DECREF(pModule);
    Py_DECREF(pRetVal);

    Py_Finalize();

    return result;
}


int iscsi_set_ACLs_auth(char wwn[], int tag, char acl_wwn[][MAXNAMELEN], char mutual_user[], char mutual_passwd[],char user[], char passwd[] ,int number)
{
    PyObject *pName, *pModule, *pDict, *pFunc, *pArgs, *pRetVal;
    int result;
    int i;
    
    Py_Initialize();
    if(!Py_IsInitialized())   
    {  
        printf("cannot find rts");
        getchar();
        return -1;  
    }

    PyRun_SimpleString("import sys");
    PyRun_SimpleString("sys.path.append('./')");

    pName = PyString_FromString(RTSLIB_WRAPPER); 
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
    pFunc = PyDict_GetItemString(pDict,"set_ACLs_mutual_auth");
    if ( !pFunc || !PyCallable_Check(pFunc) )  
    {
        printf("can't find set_ACLs_mutual_auth");  
        getchar();  
        return -1;  
    }
    for (i = 0; i < number; ++i)
    {
        pArgs = PyTuple_New(7);
        PyTuple_SetItem(pArgs, 0, Py_BuildValue("s",wwn));
        PyTuple_SetItem(pArgs, 1, Py_BuildValue("i",tag));
        PyTuple_SetItem(pArgs, 2, Py_BuildValue("s",acl_wwn[i]));
        PyTuple_SetItem(pArgs, 3, Py_BuildValue("s",mutual_user));
        PyTuple_SetItem(pArgs, 4, Py_BuildValue("s",mutual_passwd));
        PyTuple_SetItem(pArgs, 5, Py_BuildValue("s",user));
        PyTuple_SetItem(pArgs, 6, Py_BuildValue("s",passwd));
        pRetVal = PyObject_CallObject(pFunc, pArgs);
        result = PyInt_AsLong(pRetVal);
        Py_DECREF(pArgs);
        Py_DECREF(pRetVal);
        DEBUG("return 0 for set_ACLs_mutual_auth success : %d\n", result);
    }
    

    Py_DECREF(pName);
    
    Py_DECREF(pModule);
    

    Py_Finalize();
    return result;
}



int iscsi_del_ACLs_auth(char wwn[], int tag)
{

    PyObject *pName, *pModule, *pDict, *pFunc, *pArgs, *pRetVal;
    int result;
    
    Py_Initialize();
    if(!Py_IsInitialized())   
    {  
        printf("cannot find rts");
        getchar();
        return -1;  
    }

    PyRun_SimpleString("import sys");
    PyRun_SimpleString("sys.path.append('./')");

    pName = PyString_FromString(RTSLIB_WRAPPER); 
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
    pFunc = PyDict_GetItemString(pDict,"del_ACLs_mutual_auth");
    if ( !pFunc || !PyCallable_Check(pFunc) )  
    {
        printf("can't find del_ACLs_mutual_auth");  
        getchar();  
        return -1;  
    }
    pArgs = PyTuple_New(2);
    PyTuple_SetItem(pArgs, 0, Py_BuildValue("s",wwn));
    PyTuple_SetItem(pArgs, 1, Py_BuildValue("i",tag));
    pRetVal = PyObject_CallObject(pFunc, pArgs);
    result = PyInt_AsLong(pRetVal);

    DEBUG("return 0 for del_ACLs_mutual_auth success : %d\n", result);

   
    Py_DECREF(pName);
    Py_DECREF(pModule);
    Py_DECREF(pArgs);
    Py_DECREF(pRetVal);


    Py_Finalize();
    return result;

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
 * @return 0 success -1 otherwise
 *
 *
 *
 */
int iscsi_set_mutual_auth(char wwn[], int tag, char mutual_user[], char mutual_passwd[],char user[], char passwd[])
{
    PyObject *pName, *pModule, *pDict, *pFunc, *pArgs, *pRetVal;
    int result;
    
    Py_Initialize();
    if(!Py_IsInitialized())   
    {  
        printf("cannot find rts");
        getchar();
        return -1;  
    }

    PyRun_SimpleString("import sys");
    PyRun_SimpleString("sys.path.append('./')");

    pName = PyString_FromString(RTSLIB_WRAPPER); 
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
    result = PyInt_AsLong(pRetVal);
    DEBUG("return 0 for set_TPG_mutual_auth success : %d\n", result);

    Py_DECREF(pName);
    Py_DECREF(pArgs);
    Py_DECREF(pModule);
    Py_DECREF(pRetVal);

    Py_Finalize();
    return result;
}


/*
 * delete mutual authentication credential for an iscsi target
 * @wwn     iscsi target's wwn
 * @tag     iscsi target'tag index
 * 
 * @Note: The Microsoft Windows iSCSI Initiator uses its IQN as a default login name. It
 * requires the password length to be at least 12 bytes (96 bits), and it rejects passwords that
 * are too simple.
 *
 *
 * @return 0 success -1 otherwise
 *
 *
 *
 */
int iscsi_del_mutual_auth(char wwn[], int tag)
{

    PyObject *pName, *pModule, *pDict, *pFunc, *pArgs, *pRetVal;
    int result;
    
    Py_Initialize();
    if(!Py_IsInitialized())   
    {  
        printf("cannot find rts");
        getchar();
        return -1;  
    }

    PyRun_SimpleString("import sys");
    PyRun_SimpleString("sys.path.append('./')");

    pName = PyString_FromString(RTSLIB_WRAPPER); 
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
    pArgs = PyTuple_New(2);
    PyTuple_SetItem(pArgs, 0, Py_BuildValue("s",wwn));
    PyTuple_SetItem(pArgs, 1, Py_BuildValue("i",tag));
    pRetVal = PyObject_CallObject(pFunc, pArgs);
    result = PyInt_AsLong(pRetVal);
    DEBUG("return 1 for set_TPG_mutual_auth success : %d\n", result);

    Py_DECREF(pName);
    Py_DECREF(pArgs);
    Py_DECREF(pModule);
    Py_DECREF(pRetVal);

    Py_Finalize();
    return result;

}
    


/*
 * Set initiator ACLs for an iscsi target
 *
 * @wwn     iscsi target's WWN
 * @tag     tpg's index
 * @node_acls   an array of iscsi initiator's IQNs
 * @number      size of inittiator array
 *
 * @return  0 success -1 otherwise
*/
int iscsi_set_initiators(char wwn[], int tag, char node_acls[][MAXNAMELEN], int number)
{
    PyObject *pName, *pModule, *pDict, *pFunc, *pArgs, *pRetVal;
    int result;
    
    Py_Initialize();
    if(!Py_IsInitialized())   
    {  
        printf("cannot find rts");
        getchar();
        return -1;  
    }

    PyRun_SimpleString("import sys");
    PyRun_SimpleString("sys.path.append('./')");

    pName = PyString_FromString(RTSLIB_WRAPPER); 
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
        result = PyInt_AsLong(pRetVal);

        if (result == 0)
        {
             printf ("set_initiator success \n") ;
        }else if (result == -1)
        {
            printf ("set_initiator failed\n") ;
        }
        else 
        {
            iscsi_del_initiators(wwn, tag, node_acls, i);

        }

    }

    
    Py_DECREF(pName);
    Py_DECREF(pArgs);
    Py_DECREF(pModule);
    Py_DECREF(pRetVal);

    Py_Finalize();

    return result;
}

/*
 * remove initiator ACLs for an iscsi target
 *
 * @wwn     iscsi target's WWN
 * @tag     tpg's index
 * @node_acls   an array of iscsi initiator's IQNs
 * @number      size of inittiator array
 *
 * @return  0 success -1 otherwise
*/
int iscsi_del_initiators(char wwn[], int tag, char node_acls[][MAXNAMELEN], int number)
{
    PyObject *pName, *pModule, *pDict, *pFunc, *pArgs, *pRetVal;
    int result;
    
    Py_Initialize();
    if(!Py_IsInitialized())   
    {  
        printf("cannot find rts");
        getchar();
        return -1;  
    }

    PyRun_SimpleString("import sys");
    PyRun_SimpleString("sys.path.append('./')");

    pName = PyString_FromString(RTSLIB_WRAPPER); 
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
        result = PyInt_AsLong(pRetVal);
        if (result == 0)
        {
            printf ("delete_initiator success\n") ;
        }
        else if(result == -1)
        {
            printf ("delete_initiator failed\n");

        }


    }

    

    Py_DECREF(pName);
    Py_DECREF(pArgs);
    Py_DECREF(pModule);
    Py_DECREF(pRetVal);

    Py_Finalize();
    return result;

}


void show_targetcli()
{
    Py_Initialize();
    PyRun_SimpleString("import os");
    PyRun_SimpleString("os.system('targetcli ls')");
    Py_Finalize(); 
    return;  

}