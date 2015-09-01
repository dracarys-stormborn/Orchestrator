#ifndef __TYPES_H__
#define __TYPES_H__

#include <vector>
#include <map>
#include <set>
#include <string>
using namespace std;

class VirtualMachine;
class PhysicalMachine;

typedef vector<pair<string, string> > QueryParameters;
typedef vector<vector<pair<string, string> > > JSONContainer;
typedef vector<VirtualMachine> VMList;
typedef vector<PhysicalMachine> PMList;

#endif