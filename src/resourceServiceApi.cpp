#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

#include "resourceServiceApi.h"

using namespace std;

int PhysicalMachine::npms = 0;

PhysicalMachine::PhysicalMachine(const int ncpu, const int ram, const int disk) : cpu(ncpu), ram(ram), disk(disk)
{
    freeCpu = cpu;
    freeRam = ram;
    freeDisk = disk;
    pmid = ++npms;
}

int PhysicalMachine::getFreeCores() const
{
    return freeCpu;
}

int PhysicalMachine::getFreeRAM() const
{
    return freeRam;
}

int PhysicalMachine::getFreeMemBlocks() const
{
    return freeDisk;
}

int PhysicalMachine::getMaxCores() const
{
    return cpu;
}

int PhysicalMachine::getMaxRAM() const
{
    return ram;
}

int PhysicalMachine::getMaxMemBlocks() const
{
    return disk;
}

int PhysicalMachine::getId() const
{
    return pmid;
}

map<string, vector<int> > PhysicalMachineFactory::getVMList() const
{
    return vms;
}

int PhysicalMachineFactory::getPhysicalMachineIPs(string file)
{
    ifstream f(file);
    string line;
    static int id = 0;
    if(f.is_open())
    {
	while(getline(f, line)) {
	    size_t firstScan = line.find_first_not_of(' ');
	    size_t first     = firstScan == string::npos ? line.length() : firstScan;
	    size_t last      = line.find_last_not_of(' ');
	    line = line.substr(first, last-first+1);
	    p[line] = id;
	    pRev[id++] = line;
	}
    }
    else
    {
	/* Handle the error : file mentioned cannot be opened */
    }
}

map<string, int> PhysicalMachineFactory::getList() const
{
    return p;
}

map<int, string> PhysicalMachineFactory::getRevList() const
{
    return pRev;
}

void PhysicalMachineFactory::addVM(string pm, const int id)
{
    vms[pm].push_back(id);
}