#include <iostream>
#include <string>
#include <vector>

#include "virtualMachineApi.h"

using namespace std;

int VirtualMachine::nvms = 0;
int VirtualMachine::test = 0;
static int debug = 0;

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


int VirtualMachineFactory::createVirtualMachine(const string &name, const int &type, const int &pm, const int &imgid)
{
    VirtualMachine vm = VirtualMachine(name, type);
    VM v = typesOfVMs[vm.getType() - 1];
    vm.setId(++VirtualMachine::nvms);
    vm.setpmid(pm);
    vm.setImgId(imgid);
    if(debug) {
	cout << "Creation Of VM Successful" << endl;
	cout << "VM Id 	: " << vm.getId() << endl;
	cout << "PM Id 	: " << vm.getpmid() << endl;
	cout << "Image Id 	: " << vm.getImgId() << endl;
	cout << "VM type 	: " << vm.getType() << endl;
	cout << "VM Cores 	: " << v.cpu << endl;
	cout << "VM Ram 	: " << v.ram << endl;
	cout << "VM Disks 	: " << v.disk << endl;
    }
    return vm.getId();
}
