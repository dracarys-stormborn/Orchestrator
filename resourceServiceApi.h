#ifndef __RESOURCESERVICEAPI_H__
#define __RESOURCESERVICEAPI_H__

#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "virtualMachineApi.h"
#include "types.h"

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
	VMList getVMList() const;

    private:
	int freeCpu, freeRam, freeDisk;
	int cpu, ram, disk;
	int pmid;
	VMList vms;

	static int npms;
};

class PhysicalMachineFactory
{
    public:
	int getPhysicalMachineIPs(string file);
	PMList getPhysicalMachineList() const;
	map<string, int> getList() const;

    private:
	map<string, int> p;
	PMList pms;
};

#endif
