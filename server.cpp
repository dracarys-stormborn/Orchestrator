#include <iostream>
#include <string>
#include <vector>
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

using namespace Poco::Net;
using namespace Poco::Util;
using namespace Poco;
using namespace std;

typedef vector < pair < string, string > > QueryParameters;
class MyRequestHandler;
int VirtualMachineRequestHandler::count = 0;
int ResourceServiceRequestHandler::count = 0;
int ImageServiceRequestHandler::count = 0;

void VirtualMachineRequestHandler::handleVMCreationRequest(HTTPServerRequest &req, HTTPServerResponse &resp)
{
	QueryParameters q;
	VirtualMachineFactory v;
	URI u = URI(req.getURI());
	q = u.getQueryParameters();
	resp.setContentType("text/html");
	ostream &out = resp.send();
	out << "<h3> Number Of Virtual Machine related Requests : " << count << " </h3>" << endl;
	string name;
	int type, image, pm = 1;
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
	int ret = v.createVirtualMachine(name, type, pm, image);
	if(ret)
	    out << "<h3> Create Request Processed </h3>" << endl;
	else
	    out << "<h3> Create Request Failed </h3>" << endl;
}

void VirtualMachineRequestHandler::handleVMTypesRequest(HTTPServerRequest &req, HTTPServerResponse &resp)
{
	ostream &out = resp.send();
	out << "<h3> Number Of Virtual Machine related Requests : " << count << " </h3>" << endl;
	out << "Types Request Processed" << endl;
}

void VirtualMachineRequestHandler::handleVMQueryRequest(HTTPServerRequest &req, HTTPServerResponse &resp)
{
	ostream &out = resp.send();
	out << "<h3> Number Of Virtual Machine related Requests : " << count << " </h3>" << endl;
	out << "Query Request Processed" << endl;
}

void VirtualMachineRequestHandler::handleVMDestroyRequest(HTTPServerRequest &req, HTTPServerResponse &resp)
{
	ostream &out = resp.send();
	out << "<h3> Number Of Virtual Machine related Requests : " << count << " </h3>" << endl;
	out << "Destroy Request Processed" << endl;
}

void ResourceServiceRequestHandler::handlePMListRequest(HTTPServerRequest &req, HTTPServerResponse &resp)
{
	ostream &out = resp.send();
	out << "<h3> Number Of Resource Service related Requests : " << count << " </h3>" << endl;
	out << "Physical Machine's List Request Processed" << endl;
}

void ResourceServiceRequestHandler::handleVMListRequest(HTTPServerRequest &req, HTTPServerResponse &resp)
{
	ostream &out = resp.send();
	out << "<h3> Number Of Resource Service related Requests : " << count << " </h3>" << endl;
	out << "Virtual Machine's List Request Processed" << endl;
}

void ResourceServiceRequestHandler::handlePMQueryRequest(HTTPServerRequest &req, HTTPServerResponse &resp)
{
	ostream &out = resp.send();
	out << "<h3> Number Of Resource Service related Requests : " << count << " </h3>" << endl;
	out << "Physical Machine Query Request Processed" << endl;
}

void ImageServiceRequestHandler::handleImageListRequest(HTTPServerRequest &req, HTTPServerResponse &resp)
{
	ostream &out = resp.send();
	out << "<h3> Number Of Image Service related Requests : " << count << " </h3>" << endl;
	out << "Image List Request Processed" << endl;
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

int main(int argc, char** argv)
{
    MyServerApp app;
    return app.run(argc, argv);
}
