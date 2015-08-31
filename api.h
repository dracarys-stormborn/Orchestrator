#ifndef __API_H__
#define __API_H__

#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <map>

using namespace std;

class Scheduler
{
    public:
	Scheduler(map<string, int> ps) : pm(ps) {}

    private:
	map<string, int> pm;
	set<pair<int, int> > sortedPMs;

    public:
	void initializeSchedulingInfo() {}
	void updateSchedulingInfo() {}
	string getPhysicalMachineAddress(int _cpu, int _ram, int _disk) const;

	static int markedPM;
};

class System
{
    public:
	System() {}

	string exec(const char* cmd) const;

    public:
	string freeRAMInfo = " free -k | grep 'Mem:' | awk '{ print $4 }'";
	string freeCPUInfo = " grep processor /proc/cpuinfo | wc -l";
	string SSH = "ssh";
};

#endif
