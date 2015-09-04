#ifndef __API_H__
#define __API_H__

#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <map>

#include "types.h"

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
	string getPhysicalMachineAddress(int _cpu, int _ram, int _disk);

	static int markedPM;
};

class System
{
    public:
	System() {}

	string exec(const char* cmd) const;

    public:
	string diskInfo = "  df -k --total | grep total | awk '{ print $2 }'";
	string cpuInfo = "  grep processor /proc/cpuinfo | wc -l";
	string ramInfo = "  free -k | grep 'Mem:' | awk '{ print $2 }'";
	string freeDISKInfo = "  df -k --total | grep total | awk '{ print $4 }'";
	string freeRAMInfo = "  free -k | grep 'Mem:' | awk '{ print $4 }'";
	string freeCPUInfo = "  grep processor /proc/cpuinfo | wc -l";
	string SSH = "ssh";
};


class JSONHandler
{
    public:
	JSONHandler() {}

	void jsonify(string &out, JSONContainer nameValuePair);
};

class XMLHandler
{
    public:
	XMLHandler() {}

    public:
	void getDomainXML(string &xml, vector<string> args);
};

#endif
