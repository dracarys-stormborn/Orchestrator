#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

#include "resourceServiceApi.h"
#include "api.h"

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

map<string, set<int> > PhysicalMachineFactory::getVMList() const
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
    set<int> &in = vms[pm];
    in.insert(id);
}

void PhysicalMachineFactory::removeVM(const int pid, const int vid)
{
    set<int> &r = vms[pRev[pid]];
    r.erase(vid);
}

bool PhysicalMachineFactory::queryPhysicalMachine(const int pid, string &result)
{
    if(pid > p.size())
	return false;
    System s;
    JSONHandler js;
    JSONContainer status;
    string pm, r1, r2;
    string cmd, out;
    string cpu, ram, disk;
    string freeCPU, freeRAM, freeDISK;

    pm = pRev[pid - 1];
    set<int> vm = vms[pm];
    cmd = s.SSH + " " + pm + s.freeRAMInfo;
    out = s.exec(cmd.c_str());
    freeRAM = out;
    cmd = s.SSH + " " + pm + s.freeCPUInfo;
    out = s.exec(cmd.c_str());
    freeCPU = out;
    cmd = s.SSH + " " + pm + s.freeDISKInfo;
    out = s.exec(cmd.c_str());
    freeDISK = out;

    cmd = s.SSH + " " + pm + s.ramInfo;
    out = s.exec(cmd.c_str());
    ram = out;
    cmd = s.SSH + " " + pm + s.cpuInfo;
    out = s.exec(cmd.c_str());
    cpu = out;
    cmd = s.SSH + " " + pm + s.diskInfo;
    out = s.exec(cmd.c_str());
    disk = out;

    status.push_back(vector<pair<string, string> >());
    status[0].push_back(make_pair("cpu", cpu));
    status[0].push_back(make_pair("ram", ram));
    status[0].push_back(make_pair("disk", disk));
    js.jsonify(r1, status);
    status.clear();
    status.push_back(vector<pair<string, string> >());
    status[0].push_back(make_pair("cpu", freeCPU));
    status[0].push_back(make_pair("ram", freeRAM));
    status[0].push_back(make_pair("disk", freeDISK));
    js.jsonify(r2, status);
    status.clear();
    status.push_back(vector<pair<string, string> >());
    status[0].push_back(make_pair("pmid", to_string(pid)));
    status[0].push_back(make_pair("capacity", r1));
    status[0].push_back(make_pair("free", r2));
    status[0].push_back(make_pair("vms", to_string(vm.size())));
    js.jsonify(result, status);
    return true;
}