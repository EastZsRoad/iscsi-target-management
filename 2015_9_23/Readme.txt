1��Copy all the files to linux host
2��compile by makefile [root@localhost Downloads]# makefile
3��Running after compiled:[root@localhost Downloads]#./iscsitest


1��uodate 30+ Testcase include the description and result
2��fix all warning
3��replace funcion of set_the_TPG_mutual_auth with set_ACLs_mutual_auth
4��add function of del_the_ACLs_muthual_auth 
5: move wwn from attribute of sobj to attribute of target
6��fix create fileio target and block target redundancy
7��wwn can set at create target
8��fix the memory manage
9��fix the enum define
10:add define DEBUG(...) printf(...) print the debug infomation when needed

