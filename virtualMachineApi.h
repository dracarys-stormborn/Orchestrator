#ifndef __VIRTUALMACHINEAPI_H__
#define __VIRTUALMACHINEAPI_H__

#include<iostream>
#include<string>
#include<vector>

using namespace std;

class VirtualMachine
{
    public:
	VirtualMachine();
	VirtualMachine(const string name, const int instance_type);

	const string getName() const;
	const int getType() const;
	const int getId() const;
	const int getpmid() const;
	const int getImgId() const;

	void setName(const string &name);
	void setType(const int &type);
	void setId(const int &id);
	void setpmid(const int &id);
	void setImgId(const int &id);

    private:
	string vmname;
	int vmtype, vmid, pmid, imageid;

    public:
	static int nvms;
	static int test;
};

class VirtualMachineFactory
{
    protected:
	enum CPU {SINGLE_CORE = 1, DUAL_CORE = 2, QUAD_CORE = 4};
	enum RAM {LEVEL1 = 512, LEVEL2 = 1024, LEVEL3 = 2048};
	typedef struct virtualMachineTypes
	{
	    int tid;
	    CPU cpu;
	    RAM ram;
	    int disk;
	}VM;

    private:
	vector<VirtualMachine> v;

    public:
	VirtualMachineFactory() {
	    for(int i = 1; i < 4; i++) {
		VM v;
		v.tid = i;
		v.cpu = ((i == 1) ? SINGLE_CORE : ((i == 2) ? DUAL_CORE : QUAD_CORE));
		v.ram = ((i == 1) ? LEVEL1 : ((i == 2) ? LEVEL2 : LEVEL3));
		v.disk = i;
		typesOfVMs.push_back(v);
	    }
	}
	int createVirtualMachine(const string &name, const int &type, map<string, int> pm, const int &imageid);
	bool destroyVirtualMachine();
	void queryVirtualMachineList();
	void getVirtualMachineTypes();

	vector<VM> typesOfVMs;
};

#endif
