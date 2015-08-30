#ifndef __RESOURCESERVICEAPI_H__
#define __RESOURCESERVICEAPI_H__

#include <iostream>
#include <string>
#include <vector>

#include "virtualMachineApi.h"

using namespace std;

class PhysicalMachine
{
    public:
	PhysicalMachine(const int ncpu, const int ram, const int disk);

	const int getFreeCores() const;
	const int getFreeRAM() const;
	const int getFreeMemBlocks() const;
	const int getMaxCores() const;
	const int getMaxRAM() const;
	const int getMaxMemBlocks() const;
	const int getId() const;
	vector<VirtualMachine> getVMList() const;

    private:
	int freeCpu, freeRam, freeDisk;
	int cpu, ram, disk;
	int pmid;
	vector<VirtualMachine> vms;

	const int npms;
};

class PhysicalMachineFactory
{
    public:
	int getPhysicalMachineIPs(const string &file);
	vector<PhysicalMachine> getPMList() const;

    private:
	vector<string> p;
};

#endif
