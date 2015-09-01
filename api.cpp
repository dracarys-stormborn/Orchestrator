#include <stdio.h>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <map>

#include "api.h"
#include "resourceServiceApi.h"
#include "types.h"

using namespace std;

int Scheduler::markedPM = 0;

string Scheduler::getPhysicalMachineAddress(int _cpu, int _ram, int _disk)
{
    System s;
    string result, cmd;
    int cpu, ram, disk;
    for(map<string, int>::const_iterator i = pm.begin(); i != pm.end(); i++)
    {
	if(i->second == markedPM)
	{
	    cmd = s.SSH + " " + i->first + s.freeRAMInfo;
	    cout << cmd << endl;
	    result = s.exec(cmd.c_str());
	    ram = stoi(result);
	    cout << ram << endl;
	    cmd = s.SSH + " " + i->first + s.freeCPUInfo;
	    cout << cmd << endl;
	    result = s.exec(cmd.c_str());
	    cpu = stoi(result);
	    cout << cpu << endl;

	    if(ram >= _ram && cpu >= _cpu)
	    {
		return i->first;
	    }
	    markedPM = (i->second + 1) % pm.size();
	}
    }
    return "";
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

void JSONHandler::jsonify(string &out, JSONContainer nameValuePair)
{
    stringstream ss;
    int len = nameValuePair.size();
    for(int i = 0; i < len; i++)
    {
	ss << "{\n";
	for(int j = 0; j < nameValuePair[i].size(); j++)
	{
	    ss << "\"" << nameValuePair[i][j].first << "\": ";
	    ss << nameValuePair[i][j].second << ",\n";
	}
	if(i != len - 1)
	    ss << "},\n";
	else
	    ss << "}\n";
    }
    out = ss.str();
}

void XMLHandler::getDomainXML(string &xml, vector<string> args)
{
    stringstream ss;
    ss << "<domain type='" + args[0] + "' id='" + args[1] + "'>";
	ss << "<name>" + args[2] + "</name>";
	ss << "<memory>" + args[3] + "</memory>";
	ss << "<currentMemory>5000</currentMemory>";
	ss << "<vcpu>" + args[4] + "</vcpu>";
	ss << "<os>";
	    ss << "<type arch='x86_64' machine='pc-1.0'>hvm</type>";
	    ss << "<boot dev='hd'/>";
	ss << "</os>";
	ss << "<features> <acpi/> <apic/> <pae/> </features>";
	ss << "<clock offset='utc'/>";
	ss << "<on_poweroff>destroy</on_poweroff>";
	ss << "<on_reboot>restart</on_reboot>";
	ss << "<on_crash>restart</on_crash>";
	ss << "<devices>";
	    ss << "<emulator>/usr/bin/qemu-system-x86_64</emulator>";
	    ss << "<disk type='file' device='disk'>";
		ss << "<driver name='qemu' type='qcow2'/>";
		ss << "<source file='" + args[5] + "' />";
		ss << "<target dev='hda' bus='ide'/>";
		ss << "<alias name='ide0-0-0'/>";
		ss << "<address type='drive' controller='0' bus='0' unit='0'/>";
	    ss << "</disk>";
	    ss << "<controller type='ide' index='0'>";
		ss << "<alias name='ide0'/>";
		ss << "<address type='pci' domain='0x0000' bus='0x00' slot='0x01' function='0x1'/>";
	    ss << "</controller>";
	    ss << "<interface type='network'>";
		ss << "<mac address='52:54:00:82:f7:43'/>";
		ss << "<source network='default'/>";
		ss << "<target dev='vnet0'/>";
		ss << "<alias name='net0'/>";
		ss << "<address type='pci' domain='0x0000' bus='0x00' slot='0x03' function='0x0'/>";
	    ss << "</interface>";
	    ss << "<serial type='pty'>";
		ss << "<source path='/dev/pts/2'/>";
		ss << "<target port='0'/>";
		ss << "<alias name='serial0'/>";
	    ss << "</serial>";
	    ss << "<console type='pty' tty='/dev/pts/2'>";
		ss << "<source path='/dev/pts/2'/>";
		ss << "<target type='serial' port='0'/>";
		ss << "<alias name='serial0'/>";
	    ss << "</console>";
	    ss << "<input type='mouse' bus='ps2'/>";
	    ss << "<graphics type='vnc' port='5900' autoport='yes'/>";
	    ss << "<sound model='ich6'>";
		ss << "<alias name='sound0'/>";
		ss << "<address type='pci' domain='0x0000' bus='0x00' slot='0x04' function='0x0'/>";
	    ss << "</sound>";
	    ss << "<video>";
		ss << "<model type='cirrus' vram='2' heads='1'/>";
		ss << "<alias name='video0'/>";
		ss << "<address type='pci' domain='0x0000' bus='0x00' slot='0x02' function='0x0'/>";
	    ss << "</video>";
	    ss << "<memballoon model='virtio'>";
		ss << "<alias name='balloon0'/>";
		ss << "<address type='pci' domain='0x0000' bus='0x00' slot='0x05' function='0x0'/>";
	    ss << "</memballoon>";
	ss << "</devices>";
	ss << "<seclabel type='dynamic' model='apparmor' relabel='yes'>";
	    ss << "<label>libvirt-10a963ef-9458-c30d-eca3-891efd2d5817</label>";
	    ss << "<imagelabel>libvirt-10a963ef-9458-c30d-eca3-891efd2d5817</imagelabel>";
	ss << "</seclabel>";
    ss << "</domain>";
    xml = ss.str();
}