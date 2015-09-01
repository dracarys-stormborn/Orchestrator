#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <sstream>

#include <libvirt/libvirt.h>
#include "virtualMachineApi.h"
#include "api.h"
#include "types.h"

using namespace std;

int VirtualMachine::nvms = 1;
int VirtualMachine::test = 1;
static const int debug = 1;

VirtualMachine::VirtualMachine()
{
    vmname = "test_vm" + to_string(++test);
    vmtype = 1;
}

VirtualMachine::VirtualMachine(const VirtualMachine &orig)
{
    vmname = orig.getName();
    vmtype = orig.getType();
    vmid = orig.getId();
    pmid = orig.getpmid();
    imageid = orig.getImgId();
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

int VirtualMachineFactory::createVirtualMachine(const string &name, const int &type, map<string, int> ps,
	const int &imgid, string imgPath)
{
    virConnectPtr conn;
    virDomainPtr dom;
    string pm, vSystem, xml;
    XMLHandler x;
    vector<string> args;

    Scheduler s = Scheduler(ps);
    VirtualMachine vm = VirtualMachine(name, type);
    VM V = typesOfVMs[vm.getType() - 1];

    vm.setId(VirtualMachine::nvms);
    vm.setImgId(imgid);
    v.push_back(vm);

    args.push_back("qemu");
    args.push_back(to_string(vm.getId()));
    args.push_back(vm.getName());
    args.push_back(to_string(V.ram * 1000));
    args.push_back(to_string(V.cpu));
    args.push_back(imgPath);

    x.getDomainXML(xml, args);
    cout << xml << endl;

    pm = s.getPhysicalMachineAddress(V.cpu, V.ram, V.disk);
    cout << pm << endl;
    if(pm != "")
    {
	cout << ps[pm] << endl;
	vm.setpmid(ps[pm]);
	vSystem = "qemu+ssh://" + pm + "/system";
	conn = virConnectOpen(vSystem.c_str());
	cout << "Connection Successful" << endl;
	dom = virDomainDefineXML(conn, xml.c_str());
	cout << "XML Deployment Successful" << endl;
	if(dom != NULL)
	{
	    dom = virDomainLookupByName(conn, vm.getName().c_str());
	    cout << "Domain Lookup Successful" << endl;
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

bool VirtualMachineFactory::destroyVirtualMachine(const int id)
{
    return true;
}

bool VirtualMachineFactory::queryVirtualMachineList(int id, string &result)
{
    JSONHandler js;
    string name;
    int type, pid;
    JSONContainer nameValue;

    nameValue.push_back(vector<pair<string, string> >());
    if(id < v.size() + 1)
    {
	VirtualMachine vm = VirtualMachine(v[id - 1]);
	name = vm.getName();
	type = vm.getType();
	pid = vm.getpmid();
	nameValue[0].push_back(make_pair("vmid", to_string(id)));
	nameValue[0].push_back(make_pair("name", name));
	nameValue[0].push_back(make_pair("instance_type", to_string(type)));
	nameValue[0].push_back(make_pair("pmid", to_string(pid)));
	js.jsonify(result, nameValue);
	return true;
    }
    else
    {
	cout << "ERROR : VirtualMachine ID is not valid" << endl;
	return false;
    }
}

void VirtualMachineFactory::getVirtualMachineTypes(string &result)
{}

void VirtualMachineFactory::loadFlavourFile(string file)
{}