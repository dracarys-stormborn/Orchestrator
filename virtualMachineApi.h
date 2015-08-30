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

	void setName(const string &name);
	void setType(const int &type);
	void setId(const int &id);
	void setpmid(const int &id);
	void setImgId(const int &id);

    private:
	string vmname;
	int vmtype, vmid, pmid, imageid;

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
	static const VM arr[] = {{1, SINGLE_CORE, LEVEL1, 1}, 
	    {2, DUAL_CORE, LEVEL2, 2}, {3, QUAD_CORE, LEVEL3, 3}};
	vector<VM> typesOfVMs(arr, arr + sizeof(arr) / sizeof(arr[0]));
    
    public:
	int createVirtualMachine(const string &name, const int &type, const int &pmid, const int &imageid);
	bool destroyVirtualMachine();
	void queryVirtualMachineList();
	void getVirtualMachineTypes();

    private:
	vector<VirtualMachine> v;
};

#endif
