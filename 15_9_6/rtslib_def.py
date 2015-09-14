#!/usr/bin/python

from rtslib import FileIOStorageObject, BlockStorageObject

from rtslib import FabricModule, Target, TPG

from rtslib import NetworkPortal, NodeACL, LUN, MappedLUN

from rtslib import RTSRoot

from rtslib import RTSLibError, RTSLibBrokenLink

import rtslib.root as root

from rtslib.utils import fread, fwrite, RTSLibError, generate_wwn

from rtslib.utils import is_dev_in_use, get_blockdev_type, get_blockdev_size

from rtslib.utils import convert_scsi_path_to_hctl, convert_scsi_hctl_to_path

from rtslib.node import CFSNode

from ctypes import *  

import rtslib
import os
import stat
import re
import glob
import resource
import uuid
import socket
import subprocess
from contextlib import contextmanager
import json




from rtslib.tcm import so_mapping, StorageObject
from rtslib.utils import RTSLibError, RTSLibBrokenLink, modprobe, mount_configfs
from rtslib.utils import dict_remove, set_attributes


# Stor = [none] * 6
# Port = [none] * 3
# Para = [none] * 2
# Attr = [none] * 8
# Targ = [none] * 12

# class port_t(Structure):  
#     _fields_ = [("ip_address", c_char * 16), ("iser", c_int), ("port", c_int)]  
  
# if __name__ == "__main__":  
#     lib = cdll.LoadLibrary("./libtest.so")  
#     lib.test_port.restype = POINTER(port_t)  
#     p = lib.test_port()  


  
#     print "%s: %d: %d" %(p.contents.ip_address, p.contents.iser,  p.contents.port)  



def create_fileio_Target( backing_device, diskname, filesize):
	

	f = FileIOStorageObject(name = diskname, dev = backing_device,size = filesize)

	iscsiMod = FabricModule('iscsi')

	target = Target(iscsiMod, mode='create')   

	tpg = TPG(target, tag=None, mode='create')

	portal = NetworkPortal(tpg, '0.0.0.0', mode='create')

	lun = LUN(tpg, lun=None,storage_object = f)

	tpg.enable = True

	return target.wwn

def create_block_backstore( backing_device, diskname):


	try:
		fio = BlockStorageObject(name = diskname, dev = backing_device)

		iscsiMod = FabricModule('iscsi')

		target = Target(iscsiMod, mode='create')

		return target.wwn

	except Exception,e:

		print e

		return "0"



def create_tpg( backing_device, diskname):


	try:
		fio = BlockStorageObject(name = diskname, dev = backing_device)

		iscsiMod = FabricModule('iscsi')

		target = Target(iscsiMod, mode='create')

		tpg = TPG(target, tag=None, mode='create')

		tpg.set_attribute('generate_node_acls', '1')

		tpg.set_attribute('cache_dynamic_acls', '1')

		tpg.set_attribute('authentication', '0')

		tpg.set_attribute('demo_mode_write_protect', '0')

		portal = NetworkPortal(tpg, '192.168.1.55', mode='any')

		lun = LUN(tpg, lun=None, storage_object = fio)

		tpg.enable = True

		return target.wwn

	except Exception,e:
		print e
		return "0"
def create_block_Target( backing_device, diskname):


	try:
		fio = BlockStorageObject(name = diskname, dev = backing_device)

		iscsiMod = FabricModule('iscsi')

		target = Target(iscsiMod, mode='create')

		tpg = TPG(target, tag=None, mode='create')

		tpg.set_attribute('generate_node_acls', '1')

		tpg.set_attribute('cache_dynamic_acls', '1')

		tpg.set_attribute('authentication', '0')

		tpg.set_attribute('demo_mode_write_protect', '0')

		portal = NetworkPortal(tpg, '192.168.1.55', mode='any')

		lun = LUN(tpg, lun=None, storage_object = fio)

		tpg.enable = True

		return target.wwn

	except Exception,e:
		print e
		return "0"





	

def removeTarget(iqn):

	iscsiMod = rtslib.FabricModule('iscsi')

	tgt = rtslib.Target(iscsiMod,iqn,'lookup')

	tgt.delete()

	print ("the %s has been deleted" %(iqn))


def remove_portals(iqn ,tag_index ,ip_address, port):

	iscsiMod = FabricModule('iscsi')

	target = Target(iscsiMod, iqn, mode='lookup')

	tpg = TPG(target, tag = tag_index, mode = 'any')

	tpg.enable = True

	portal = NetworkPortal(tpg, ip_address, port, mode='lookup')

	portal.delete()




def create_initiator_auth(iqn, tag_index, new_iqn, userid, password):

	iscsiMod = FabricModule('iscsi')

	target = Target(iscsiMod, iqn, mode = 'lookup')

	print list(target.tpgs)

	tpg = TPG(target, tag = tag_index, mode = 'any')

	tpg.enable = True
	
	tpg.set_attribute('authentication', '1')

	nodeacl = NodeACL(tpg, 'new_iqn')

	nodeacl.chap_userid = userid

	nodeacl.chap_password = password





def creat_mutual_auth(iqn, tag_index, new_iqn, mutual_userid, mutual_password):

	iscsiMod = FabricModule('iscsi')

	target = Target(iscsiMod, iqn, mode = 'lookup')

	print list(target.tpgs)

	tpg = TPG(target, tag = tag_index, mode = 'any')

	tpg.enable = True
	
	tpg.set_attribute('authentication', '1')

	nodeacl = NodeACL(tpg, 'new_iqn')

	nodeacl.chap_mutual_userid = mutual_userid

	nodeacl.chap_mutual_password = mutual_password



def creat_TPG_auth(iqn, tag_index, userid, password, mutual_userid, mutual_password):

	iscsiMod = FabricModule('iscsi')

	iscsiMod._set_discovery_enable_auth(enable)

	iscsiMod._set_discovery_userid(userid)

	iscsiMod._set_discovery_password(password)

	target = Target(iscsiMod, iqn, mode = 'lookup')

	print list(target.tpgs)

	tpg = TPG(target, tag = tag_index, mode = 'any')

	tpg.enable = True
	
	tpg.set_attribute('authentication', '1')

	tpg.chap_userid = userid

	tpg.chap_password = password

	tpg.chap_mutual_userid = mutual_userid

	tpg.chap_mutual_password = mutual_password



def creat_discovery_auth(userid, password):

	iscsiMod = FabricModule('iscsi')

	iscsiMod._set_discovery_enable_auth(enable)

	iscsiMod._set_discovery_userid(userid)

	iscsiMod._set_discovery_password(password)




def creat_mutual_discovery_auth(mutual_userid, mutual_password):


	iscsiMod = FabricModule('iscsi')

	iscsiMod._set_discovery_enable_auth(enable)

	iscsiMod._set_discovery_userid(mutual_userid)

	iscsiMod._set_discovery_password(mutual_password)





def create_portals(iqn ,tag_index ,ip_address, port):

	iscsiMod = FabricModule('iscsi')

	target = Target(iscsiMod, iqn, mode='lookup')

	print list(target.tpgs)

	tpg = TPG(target, tag = tag_index, mode = 'any')

	tpg.enable = True

	portal = NetworkPortal(tpg, ip_address, port, mode='create')


def modify_block_size(block_diskname,set_block_size):

	fio = BlockStorageObject(name = block_diskname)

	fio.set_attribute('block_size','set_block_size')

	return fio.get_attribute('block_size')


def delete_all_targets():
	rtsroot = root.RTSRoot()
	print "Before clearconfigs"
	print list(rtsroot.storage_objects)
	print list(rtsroot.targets)
	

	rtsroot.clear_existing(True)


	print "after clearconfigs"
	print list(rtsroot.storage_objects)
	print list(rtsroot.targets)

	return 1
	

	
	

