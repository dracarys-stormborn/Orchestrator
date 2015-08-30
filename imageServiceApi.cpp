#include <iostream>
#include <vector>
#include <string>

using namespace std;

OperatingSystem::OperatingSystem(const string &name) : imgname(name) { osid = 100 + (++nimgs); }

void OperatingSystemFactory::createImageList(const string &file)
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
	    imgPath.push_back(line.substr(line.find_first_of(":"), line.length()));
	    img.push_back(line.substr(line.find_last_of("/"), line.length()));
	}
    }
    else
    {
	/* Handle file I/O exceptions */
    }
}

vector<string> getImageList() const
{
    return img;
}
