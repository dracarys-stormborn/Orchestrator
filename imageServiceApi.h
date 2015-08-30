#ifndef __IMAGESERVICEAPI_H__
#define __IMAGESERVICEAPI_H__

#include <iostream>
#include <vector>
#include <string>

using namespace std;

class OperatingSystem
{
    public:
	OperatingSystem(const string &name);

    private:
	string imgname;
	int osid;

	static int nimgs;
};

class OperatingSystemFactory
{
    public:
	void createImageList(const string &file);
	vector<string> getImageList() const;

    private:
	vector<string> img;
	vector<string> imgPath;
};

#endif
