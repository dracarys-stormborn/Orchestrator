#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

#include "imageServiceApi.h"

int OperatingSystem::nimgs = 0;

OperatingSystem::OperatingSystem(const string &name) : imgname(name) { osid = 100 + (++nimgs); }

void OperatingSystemFactory::createImageList(string file)
{
    ifstream f(file);
    string line;
    if(f.is_open())
    {
	while(getline(f, line))
	{
	    size_t firstScan = line.find_first_not_of(' ');
	    size_t first     = firstScan == string::npos ? line.length() : firstScan;
	    size_t last      = line.find_last_not_of(' ');
	    line = line.substr(first, last-first+1);
	    imgPath.push_back(line.substr(line.find_first_of(":") + 1, line.length()));
	    img.push_back(line.substr(line.find_last_of("/") + 1, line.length()));
	    ++OperatingSystem::nimgs;
	}
    }
    else
    {
	/* Handle file I/O exceptions */
    }
}

vector<string> OperatingSystemFactory::getImageList() const
{
    return img;
}

vector<string> OperatingSystemFactory::getImagePathList() const
{
    return imgPath;
}
