#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "Poco/Net/ServerSocket.h"
#include "Poco/Net/HTTPServer.h"
#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Net/HTTPRequestHandlerFactory.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Util/ServerApplication.h"
#include "Poco/URI.h"
#include "server.h"
#include "virtualMachineApi.h"
#include "resourceServiceApi.h"
#include "imageServiceApi.h"
#include "api.h"
#include "types.h"

using namespace Poco::Net;
using namespace Poco::Util;
using namespace Poco;
using namespace std;

class MyRequestHandler;
int VirtualMachineRequestHandler::count = 0;
int ResourceServiceRequestHandler::count = 0;
int ImageServiceRequestHandler::count = 0;

static const int debug = 1;

extern VirtualMachineFactory _vm;
extern PhysicalMachineFactory _pm;
extern OperatingSystemFactory _os;

void VirtualMachineRequestHandler::handleVMCreationRequest(HTTPServerRequest &req, HTTPServerResponse &resp)
{
    QueryParameters q;
    string name, pm, result;
    vector<string> imgPathList;
    int type, image, ret;
    JSONContainer status;
    JSONHandler js;

    URI u = URI(req.getURI());
    q = u.getQueryParameters();
    resp.setContentType("application/json");

    ostream &out = resp.send();
    status.push_back(vector<pair<string, string> >());

    for(int i = 0; i < q.size(); i++)
    {
	if(q[i].first.compare("name") == 0) {
	    name = q[i].second;
	}
	else if(q[i].first.compare("instance_type") == 0) {
	    type = stoi(q[i].second);
	}
	else {
	    image = stoi(q[i].second);
	}
    }

    imgPathList = _os.getImagePathList();
    ret = _vm.createVirtualMachine(name, type, _pm.getList(), image, imgPathList[image - 1]);
    if(ret) {
	status[0].push_back(make_pair("vmid", to_string(ret)));
	js.jsonify(result, status);
	out << result;
    }
    else {
	status[0].push_back(make_pair("vmid", "0"));
	js.jsonify(result, status);
	out << result;
    }
    out.flush();
}

void VirtualMachineRequestHandler::handleVMTypesRequest(HTTPServerRequest &req, HTTPServerResponse &resp)
{
    string result;

    resp.setContentType("application/json");

    _vm.getVirtualMachineTypes(result);
    ostream &out = resp.send();
    out << result;
    out.flush();
}

void VirtualMachineRequestHandler::handleVMQueryRequest(HTTPServerRequest &req, HTTPServerResponse &resp)
{
    QueryParameters q;
    string result;
    int vmid;
    bool done = false;
    JSONContainer status;
    JSONHandler js;

    URI u = URI(req.getURI());
    q = u.getQueryParameters();
    resp.setContentType("application/json");
    status.push_back(vector<pair<string, string> >());

    ostream &out = resp.send();
    vmid = stoi(q[0].second);
    done = _vm.queryVirtualMachineList(vmid, result);
    if(done)
    {
	out << result;
    }
    else
    {
	status[0].push_back(make_pair("status", "VMID is invalid. Please enter a valid vmid in the query"));
	js.jsonify(result, status);
	out << result;
    }
    out.flush();
}

void VirtualMachineRequestHandler::handleVMDestroyRequest(HTTPServerRequest &req, HTTPServerResponse &resp)
{
    QueryParameters q;
    string result;
    int vmid;
    bool done = false;
    JSONContainer status;
    JSONHandler js;

    URI u = URI(req.getURI());
    q = u.getQueryParameters();
    resp.setContentType("application/json");
    status.push_back(vector<pair<string, string> >());

    ostream &out = resp.send();
    vmid = stoi(q[0].second);
    done = _vm.destroyVirtualMachine(vmid);
    if(done)
    {
	status[0].push_back(make_pair("status", "VM domain destroyed successfully from the hypervisor"));
	js.jsonify(result, status);
	out << result;
    }
    else
    {
	status[0].push_back(make_pair("status", "VMID is invalid. Please enter a valid vmid in the query"));
	js.jsonify(result, status);
	out << result;
    }
    out.flush();
}

void ResourceServiceRequestHandler::handlePMListRequest(HTTPServerRequest &req, HTTPServerResponse &resp)
{
    string result, array;
    JSONContainer status;
    JSONHandler js;

    map<string, int> p = _pm.getList();
    map<string, int>::iterator temp = p.end();

    array = "[";
    for(map<string, int>::iterator i = p.begin(); i != p.end(); i++)
    {
	array += to_string(i->second + 1);
	if(i != (--temp))
	    array += ", ";
    }
    array += "]";

    status.push_back(vector<pair<string, string> >());
    status[0].push_back(make_pair("Physical Machine IDs", array));
    js.jsonify(result, status);

    ostream &out = resp.send();
    out << result;
    out.flush();
}

void ResourceServiceRequestHandler::handleVMListRequest(HTTPServerRequest &req, HTTPServerResponse &resp)
{
    QueryParameters q;
    string result, array;
    JSONContainer status;
    JSONHandler js;

    URI u = URI(req.getURI());
    q = u.getQueryParameters();
    map<string, set<int> > p = _pm.getVMList();
    map<int, string> pr = _pm.getRevList();
    string pm = pr[stoi(q[0].second) - 1];
    set<int> vm = p[pm];
    set<int>::iterator it;
    set<int>::iterator temp = vm.end();

    array = "[";
    for(set<int>::iterator i = vm.begin(); i != vm.end(); i++)
    {
	array += to_string(*i);
	if(i != (--temp))
	    array += ", ";
    }
    array += "]";

    status.push_back(vector<pair<string, string> >());
    status[0].push_back(make_pair("Virtual Machine IDs which are running on queried Physical Machine", array));
    js.jsonify(result, status);

    ostream &out = resp.send();
    out << result;
    out.flush();
}

void ResourceServiceRequestHandler::handlePMQueryRequest(HTTPServerRequest &req, HTTPServerResponse &resp)
{
    QueryParameters q;
    string result;
    int pid;
    bool done = false;

    URI u = URI(req.getURI());
    q = u.getQueryParameters();
    pid = stoi(q[0].second);
    ostream &out = resp.send();
    done = _pm.queryPhysicalMachine(pid, result);
    if(done)
	out << result;
    else
    {
	out << "{\n";
	out << "\"status\" : Enter a valid Physical Machine ID\n";
	out << "}";
    }
    out.flush();
}

void ImageServiceRequestHandler::handleImageListRequest(HTTPServerRequest &req, HTTPServerResponse &resp)
{
    string result, array;
    JSONContainer status;
    JSONHandler js;
    vector<string> images;

    images = _os.getImageList();
    array = "[\n";
    for(int i = 0; i < images.size(); i++)
    {
	string temp;
	status.push_back(vector<pair<string, string> >());
	status[0].push_back(make_pair("id", to_string(i + 100)));
	status[0].push_back(make_pair("name", images[i]));
	js.jsonify(temp, status);
	if(i != images.size() - 1)
	    array += temp + ",\n";
	else
	    array += temp;
	status.clear();
    }
    array += "\n]";

    status.push_back(vector<pair<string, string> >());
    status[0].push_back(make_pair("images", array));
    js.jsonify(result, status);

    ostream &out = resp.send();
    out << result;
    out.flush();
}

class MyRequestHandlerFactory : public HTTPRequestHandlerFactory
{
    public:
	virtual HTTPRequestHandler* createRequestHandler(const HTTPServerRequest &)
	{
	    return new MyRequestHandler;
	}
};

class MyServerApp : public ServerApplication
{
    protected:
	int main(const vector<string> &)
	{
	    HTTPServer s(new MyRequestHandlerFactory, ServerSocket(5000), new HTTPServerParams);

	    s.start();
	    cout << endl << "Server started" << endl;

	    waitForTerminationRequest();  // wait for CTRL-C or kill

	    cout << endl << "Shutting down..." << endl;
	    s.stop();

	    return Application::EXIT_OK;
	}
};

int main(int argc, char* argv[])
{
    MyServerApp app;
    _pm.getPhysicalMachineIPs(argv[1]);
    _os.createImageList(argv[2]);
    if(debug)
    {
	map<string, int> a = _pm.getList();
	vector<string> b = _os.getImageList();
	for(map<string, int>::iterator i = a.begin(); i != a.end(); i++)
	    cout << i->first << endl;
	for(int i = 0; i < b.size(); i++)
	    cout << b[i] << endl;
    }
    return app.run(argc, argv);
}
