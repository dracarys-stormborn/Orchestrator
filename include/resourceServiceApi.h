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

	int getFreeCores() const;
	int getFreeRAM() const;
	int getFreeMemBlocks() const;
	int getMaxCores() const;
	int getMaxRAM() const;
	int getMaxMemBlocks() const;
	int getId() const;

    private:
	int freeCpu, freeRam, freeDisk;
	int cpu, ram, disk;
	int pmid;

	static int npms;
};

class PhysicalMachineFactory
{
    public:
	int getPhysicalMachineIPs(string file);
	PMList getPhysicalMachineList() const;
	map<string, int> getList() const;
	map<int, string> getRevList() const;
	map<string, vector<int> > getVMList() const;
	void addVM(string pm, const int id);

    private:
	map<string, int> p;
	map<int, string> pRev;
	map<string, vector<int> > vms;
	PMList pms;
};

#endif
