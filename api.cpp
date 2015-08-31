#include <stdio.h>
#include <iostream>
#include <vector>
#include <string>
#include <map>

#include "api.h"
#include "resourceServiceApi.h"

using namespace std;

int Scheduler::markedPM = 0;

string Scheduler::getPhysicalMachineAddress(int _cpu, int _ram, int _disk) const
{
    return "dracarys983@10.1.35.196";
    System s;
    string result, cmd;
    int cpu, ram, disk;
    for(map<string, int>::const_iterator i = pm.begin(); i != pm.end(); i++)
    {
	if(i->second == markedPM)
	{
	    cmd = s.SSH + i->first + s.freeRAMInfo;
	    result = s.exec(cmd.c_str());
	    ram = stoi(result);
	    cmd = s.SSH + i->first + s.freeCPUInfo;
	    result = s.exec(cmd.c_str());
	    cpu = stoi(result);

	    if(ram >= _ram && cpu >= _cpu)
	    {
		return i->first;
	    }
	    markedPM = (i->second + 1) % pm.size();
	}
    }
}

string System::exec(const char* cmd) const
{
    FILE* pipe = popen(cmd, "r");
    if (!pipe) return "ERROR";
    char buffer[128];
    string result = "";
    while(!feof(pipe)) {
	if(fgets(buffer, 128, pipe) != NULL)
	    result += buffer;
    }
    pclose(pipe);
    return result;
}
