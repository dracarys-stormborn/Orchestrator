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

const int PhysicalMachine::getFreeCores() const
{
    return freeCpu;
}

const int PhysicalMachine::getFreeRAM() const
{
    return freeRam;
}

const int PhysicalMachine::getFreeMemBlocks() const
{
    return freeDisk;
}

const int PhysicalMachine::getMaxCores() const
{
    return cpu;
}

const int PhysicalMachine::getMaxRAM() const
{
    return ram;
}

const int PhysicalMachine::getMaxMemBlocks() const
{
    return disk;
}

const int PhysicalMachine::getId() const
{
    return pmid;
}

vector<VirtualMachine> PhysicalMachine::getVMList() const
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
	    p[line] = id++;
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