#include <iostream>
#include <string>
#include <vector>

using namespace std;

int VirtualMachine::nvms = 0;
int VirtualMachine::test = 0;

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


int VirtualMachineFactory::createVirtualMachine(const int imgid)
{
    VM v = typesOfVMs[vmtype - 1];
    imageid = imgid;
}
