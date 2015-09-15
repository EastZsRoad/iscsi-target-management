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

from rtslib.tcm import so_mapping, StorageObject

from rtslib.utils import RTSLibError, RTSLibBrokenLink, modprobe, mount_configfs

from rtslib.utils import dict_remove, set_attributes

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






def create_fileio_Target( backing_device, diskname, filesize):
	

	fio = FileIOStorageObject(name = diskname, dev = backing_device,size = filesize)

	iscsiMod = FabricModule('iscsi')

	target = Target(iscsiMod, mode='create')   

	tpg = TPG(target, tag=None, mode='create')

	portal = NetworkPortal(tpg, '0.0.0.0', mode='create')

	lun = LUN(tpg, lun=None,storage_object = fio)

	tpg.enable = True

	return target.wwn

def create_block_backstore( backing_device, diskname):

	try:
		bio = BlockStorageObject(name = diskname, dev = backing_device)

		print ("the blockmajor is %i" %bio.major)

		print ("the blocksize is %i" %bio.size)


		return 0

	except Exception,e:

		print e

		return  -1

		
def create_target():

	iscsiMod = FabricModule('iscsi')

	target = Target(iscsiMod, mode='create')

	return target.wwn






def create_tpg(diskname, iqn, ip_address, port):

	try:

		bio = BlockStorageObject(name = diskname)

		iscsiMod = FabricModule('iscsi')

		target = Target(iscsiMod, wwn = iqn, mode='lookup')

		tpg = TPG(target, tag=None, mode='create')

		lun = LUN(tpg, 0, bio)

		portal = NetworkPortal(tpg, ip_address, port, mode='any')

		return 0

	except Exception,e:

		print e

		return -1


def enable_tpg(iqn, tag_index , enable):

	try:
		iscsiMod = FabricModule('iscsi')

		target = Target(iscsiMod, wwn = iqn, mode='lookup')

		tpg = TPG(target, tag = tag_index, mode='lookup')

		tpg.enable = True

		return 0

	except Exception,e:

		print e

		return -1





def set_tpg_attr(iqn, tag_index, auth, cach, defa, demo, gene, logi, neti, prod):

	try:
		iscsiMod = FabricModule('iscsi')

		target = Target(iscsiMod, wwn = iqn, mode='lookup')

		tpg = TPG(target, tag = tag_index, mode='lookup')

		tpg.set_attribute('authentication', auth)

		tpg.set_attribute('cache_dynamic_acls', cach)
		
		tpg.set_attribute('default_cmdsn_depth', defa)

		tpg.set_attribute('demo_mode_write_protect', demo)

		tpg.set_attribute('generate_node_acls', gene)

		tpg.set_attribute('login_timeout', logi)

		tpg.set_attribute('netif_timeout', neti)

		tpg.set_attribute('prod_mode_write_protect', prod)

		return 0

	except Exception, e:

		print e
 		
 		return -1




def delete_target(iqn):

	try:
		rtsroot = root.RTSRoot()

		iscsiMod = rtslib.FabricModule('iscsi')

		tgt = rtslib.Target(iscsiMod,iqn,'lookup')

		tgt.delete()

		print ("the %s has been deleted" %(iqn))

		print list(rtsroot.targets)

		return 0

	except Exception,e:

		print e

		return 1


def delete_backstore(diskname):

	try:
		io = BlockStorageObject(name = diskname)

		io.delete()

		print ("the %s has been deleted" %(diskname))

		return 0

	except Exception,e:

		print e

		return 1
	


def remove_portals(iqn ,tag_index ,ip_address, port):

	iscsiMod = FabricModule('iscsi')

	target = Target(iscsiMod, iqn, mode='lookup')

	tpg = TPG(target, tag = tag_index, mode = 'any')

	tpg.enable = True

	portal = NetworkPortal(tpg, ip_address, port, mode='lookup')

	portal.delete()


def set_initiator(iqn, tag_index, initi_wwn):

	try:
		iscsiMod = FabricModule('iscsi')

		target = Target(iscsiMod, iqn, mode='lookup')

		tpg = TPG(target, tag = tag_index, mode = 'any')

		node = NodeACL(tpg, node_wwn = initi_wwn, mode='create')

		tpg.set_attribute('generate_node_acls', 0)

		maplun = MappedLUN(parent_nodeacl = node,mapped_lun = 0,tpg_lun=0)

		return 1

	except Exception, e:

		print e

		return 0

def delete_initiator(iqn, tag_index, initi_wwn):

	try:
		iscsiMod = FabricModule('iscsi')

		target = Target(iscsiMod, iqn, mode='lookup')

		tpg = TPG(target, tag = tag_index, mode = 'lookup')

		node = NodeACL(tpg, node_wwn = initi_wwn, mode='lookup')

		node.delete()

		return 1

	except Exception, e:

		print e

		return 0
	




'''
# def create_initiator_auth(iqn, tag_index, new_iqn, userid, password):

# 	iscsiMod = FabricModule('iscsi')

# 	target = Target(iscsiMod, iqn, mode = 'lookup')

# 	print list(target.tpgs)

# 	tpg = TPG(target, tag = tag_index, mode = 'any')

# 	tpg.enable = True
	
# 	tpg.set_attribute('authentication', '1')

# 	nodeacl = NodeACL(tpg, 'new_iqn')

# 	nodeacl.chap_userid = userid

# 	nodeacl.chap_password = password





# def creat_mutual_auth(iqn, tag_index, new_iqn, mutual_userid, mutual_password):

# 	iscsiMod = FabricModule('iscsi')

# 	target = Target(iscsiMod, iqn, mode = 'lookup')

# 	print list(target.tpgs)

# 	tpg = TPG(target, tag = tag_index, mode = 'any')

# 	tpg.enable = True
	
# 	tpg.set_attribute('authentication', '1')

# 	nodeacl = NodeACL(tpg, 'new_iqn')

# 	nodeacl.chap_mutual_userid = mutual_userid

# 	nodeacl.chap_mutual_password = mutual_password

'''

def set_TPG_mutual_auth(iqn, tag_index , userid, password, mutual_userid, mutual_password):

	try:
		iscsiMod = FabricModule('iscsi')

		target = Target(iscsiMod, iqn, mode = 'lookup')

		tpg = TPG(target, tag = tag_index , mode = 'lookup')

		tpg.enable = True
		
		tpg.set_attribute('authentication', '1')

		tpg.chap_userid = userid

		tpg.chap_password = password

		tpg.chap_mutual_userid = mutual_userid

		tpg.chap_mutual_password = mutual_password

		return 1

	except Exception,e:

		print e

		return 0

	


'''

# def creat_discovery_auth(userid, password):

# 	iscsiMod = FabricModule('iscsi')

# 	iscsiMod._set_discovery_enable_auth(enable)

# 	iscsiMod._set_discovery_userid(userid)

# 	iscsiMod._set_discovery_password(password)




# def creat_mutual_discovery_auth(mutual_userid, mutual_password):


# 	iscsiMod = FabricModule('iscsi')

# 	iscsiMod._set_discovery_enable_auth(enable)

# 	iscsiMod._set_discovery_userid(mutual_userid)

# 	iscsiMod._set_discovery_password(mutual_password)





# def create_portals(iqn ,tag_index ,ip_address, port):

# 	iscsiMod = FabricModule('iscsi')

# 	target = Target(iscsiMod, iqn, mode='lookup')

# 	print list(target.tpgs)

# 	tpg = TPG(target, tag = tag_index, mode = 'any')

# 	tpg.enable = True

# 	portal = NetworkPortal(tpg, ip_address, port, mode='create')

'''
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
	

	
	

