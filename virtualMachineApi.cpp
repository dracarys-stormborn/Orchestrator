#include <iostream>
#include <string>
#include <vector>
#include <map>

#include <libvirt/libvirt.h>
#include "virtualMachineApi.h"
#include "api.h"

using namespace std;

int VirtualMachine::nvms = 1;
int VirtualMachine::test = 1;
static const int debug = 1;

VirtualMachine::VirtualMachine()
{
    vmname = "test_vm" + to_string(++test);
    vmtype = 1;
}

VirtualMachine::VirtualMachine(const string name, const int instance_type) : vmname(name), vmtype(instance_type) {}

const string VirtualMachine::getName() const
{
    return vmname;
}
const int VirtualMachine::getType() const
{
    return vmtype;
}
const int VirtualMachine::getId() const
{
    return vmid;
}
const int VirtualMachine::getpmid() const
{
    return pmid;
}
const int VirtualMachine::getImgId() const
{
    return imageid;
}

void VirtualMachine::setName(const string &name)
{
    vmname = name;
}
void VirtualMachine::setType(const int &type)
{
    vmtype = type;
}
void VirtualMachine::setId(const int &id)
{
    vmid = id;
}
void VirtualMachine::setpmid(const int &id)
{
    pmid = id;
}
void VirtualMachine::setImgId(const int &id)
{
    imageid = id;
}

int VirtualMachineFactory::createVirtualMachine(const string &name, const int &type, map<string, int> ps, const int &imgid)
{
    virConnectPtr conn;
    virDomainPtr dom;
    string pm, vSystem, xml;

    Scheduler s = Scheduler(ps);
    VirtualMachine vm = VirtualMachine(name, type);
    VM V = typesOfVMs[vm.getType() - 1];

    vm.setId(VirtualMachine::nvms);
    vm.setImgId(imgid);

    pm = s.getPhysicalMachineAddress(V.cpu, V.ram, V.disk);
    if(pm != "")
    {
	vm.setpmid(ps[pm]);
	vSystem = "qemu+ssh://" + pm + "/system";
	conn = virConnectOpen(vSystem.c_str());
	dom = virDomainDefineXML(conn, xml.c_str());
	if(dom != NULL)
	{
	    dom = virDomainLookupByName(conn, vm.getName().c_str());
	    if(dom != NULL)
	    {
		int ret = virDomainCreate(dom);
		if(ret == -1)
		    return 0;
	    }
	    else
	    {
		return 0;
	    }
	}
	else
	{
	    return 0;
	}
	v.push_back(vm);
    }
    if(debug) {
	cout << "Creation Of VM Successful" << endl;
	cout << "VM Id 	: " << vm.getId() << endl;
	cout << "PM Id 	: " << vm.getpmid() << endl;
	cout << "Image Id 	: " << vm.getImgId() << endl;
	cout << "VM type 	: " << vm.getType() << endl;
	cout << "VM Cores 	: " << V.cpu << endl;
	cout << "VM Ram 	: " << V.ram << endl;
	cout << "VM Disks 	: " << V.disk << endl;
    }
    ++VirtualMachine::nvms;
    return vm.getId();
}
