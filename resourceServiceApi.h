#ifndef __RESOURCESERVICEAPI_H__
#define __RESOURCESERVICEAPI_H__

#include <iostream>
#include <string>
#include <vector>
#include <map>

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

	static int npms;
};

class PhysicalMachineFactory
{
    public:
	int getPhysicalMachineIPs(string file);
	vector<PhysicalMachine> getPhysicalMachineList() const;
	map<string, int> getList() const;

    private:
	map<string, int> p;
	vector<PhysicalMachine> pms;
};

#endif
