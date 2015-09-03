#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <cstdlib>

#include <libvirt/libvirt.h>
#include "virtualMachineApi.h"
#include "api.h"
#include "types.h"
#include "resourceServiceApi.h"
#include "imageServiceApi.h"

using namespace std;

VirtualMachineFactory _vm;
PhysicalMachineFactory _pm;
OperatingSystemFactory _os;

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

string VirtualMachine::getName() const
{
    return vmname;
}
int VirtualMachine::getType() const
{
    return vmtype;
}
int VirtualMachine::getId() const
{
    return vmid;
}
int VirtualMachine::getpmid() const
{
    return pmid;
}
int VirtualMachine::getImgId() const
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

int VirtualMachineFactory::createVirtualMachine(string name, const int &type, map<string, int> ps,
	const int &imgid, string imgPath)
{
    virConnectPtr conn;
    virDomainPtr dom;
    int gen;
    string pm, vSystem, xml;
    XMLHandler x;
    vector<string> args;

    Scheduler s = Scheduler(ps);
    VirtualMachine vm = VirtualMachine(name, type);
    VM V = typesOfVMs[vm.getType() - 1];

    gen = rand() % 137;
    while(allocedIds.find(gen) != allocedIds.end())
	gen = (gen + 1) % 137;
    allocedIds.insert(gen);
    vm.setId(gen);
    vm.setImgId(imgid);

    args.push_back("qemu");
    args.push_back(to_string(vm.getId()));
    args.push_back(vm.getName());
    args.push_back(to_string(V.ram * 1000));
    args.push_back(to_string(V.cpu));
    args.push_back(imgPath);

    x.getDomainXML(xml, args);

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
	idMap[vm.getId()] = v.size() - 1;
	_pm.addVM(pm, vm.getId());
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
    virConnectPtr conn;
    virDomainPtr dom;
    string pm, vSystem;

    map<int, string> idToIP = _pm.getRevList();
    if(allocedIds.find(id) != allocedIds.end())
	pm = idToIP[v[idMap[id]].getpmid()];
    else
	return false;
    vSystem = "qemu+ssh://" + pm + "/system";
    conn = virConnectOpen(vSystem.c_str());
    dom = virDomainLookupByName(conn, v[idMap[id]].getName().c_str());

    if(dom != NULL)
    {
	int ret = virDomainDestroyFlags(dom, VIR_DOMAIN_DESTROY_GRACEFUL);
	if(ret == -1)
	    return false;
	virDomainUndefine(dom);
	v.erase(v.begin() + idMap[id]);
	allocedIds.erase(id);
    }
    else
    {
	return false;
    }
    return true;
}

bool VirtualMachineFactory::queryVirtualMachineList(int id, string &result)
{
    JSONHandler js;
    string name;
    int type, pid;
    JSONContainer nameValue;

    nameValue.push_back(vector<pair<string, string> >());
    if(allocedIds.find(id) != allocedIds.end())
    {
	name = v[idMap[id]].getName();
	type = v[idMap[id]].getType();
	pid = v[idMap[id]].getpmid() + 1;
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
{
    JSONHandler js;
    JSONContainer nameValue;

    for(int i = 0; i < typesOfVMs.size(); i++)
	nameValue.push_back(vector<pair<string, string> >());
    for(int i = 0; i < typesOfVMs.size(); i++)
    {
	nameValue[i].push_back(make_pair("tid", to_string(i + 1)));
	nameValue[i].push_back(make_pair("cpu", to_string(typesOfVMs[i].cpu)));
	nameValue[i].push_back(make_pair("ram", to_string(typesOfVMs[i].ram)));
	nameValue[i].push_back(make_pair("disk", to_string(typesOfVMs[i].disk)));
    }
    js.jsonify(result, nameValue);
    cout << result << endl;
}

void VirtualMachineFactory::loadFlavourFile(string file)
{}
