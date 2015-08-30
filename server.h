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

using namespace Poco::Net;
using namespace Poco::Util;
using namespace Poco;
using namespace std;

typedef vector < pair < string, string > > QueryParameters;

class VirtualMachineRequestHandler : public HTTPRequestHandler
{
    public:
	virtual void handleRequest(HTTPServerRequest &req, HTTPServerResponse &resp)
	{
	    ++count;
	    resp.setStatus(HTTPResponse::HTTP_OK);

	    vector<string> v;
	    URI u = URI(req.getURI());
	    u.getPathSegments(v);

	    if(v[1].compare("create") == 0) {
		handleVMCreationRequest(req, resp);
	    }
	    else if(v[1].compare("query") == 0) {
		handleVMQueryRequest(req, resp);
	    }
	    else if(v[1].compare("destroy") == 0) {
		handleVMDestroyRequest(req, resp);
	    }
	    else {
		handleVMTypesRequest(req, resp);
	    }
	}

	void handleVMCreationRequest(HTTPServerRequest &req, HTTPServerResponse &resp);
	void handleVMTypesRequest(HTTPServerRequest &req, HTTPServerResponse &resp);
	void handleVMQueryRequest(HTTPServerRequest &req, HTTPServerResponse &resp);
	void handleVMDestroyRequest(HTTPServerRequest &req, HTTPServerResponse &resp);

    private:
	static int count;
};

class ResourceServiceRequestHandler : public HTTPRequestHandler
{
    public:
	virtual void handleRequest(HTTPServerRequest &req, HTTPServerResponse &resp)
	{
	    ++count;
	    resp.setStatus(HTTPResponse::HTTP_OK);

	    vector<string> v;
	    URI u = URI(req.getURI());
	    u.getPathSegments(v);

	    if(v[1].compare("list") == 0) {
		handlePMListRequest(req, resp);
	    }
	    else if(v[1].compare("listvms") == 0) {
		handleVMListRequest(req, resp);
	    }
	    else {
		handlePMQueryRequest(req, resp);
	    }
	}

	void handlePMListRequest(HTTPServerRequest &req, HTTPServerResponse &resp);
	void handleVMListRequest(HTTPServerRequest &req, HTTPServerResponse &resp);
	void handlePMQueryRequest(HTTPServerRequest &req, HTTPServerResponse &resp);

    private:
	static int count;
};

class ImageServiceRequestHandler : public HTTPRequestHandler
{
    public:
	virtual void handleRequest(HTTPServerRequest &req, HTTPServerResponse &resp)
	{
	    ++count;
	    resp.setStatus(HTTPResponse::HTTP_OK);

	    vector<string> v;
	    URI u = URI(req.getURI());
	    u.getPathSegments(v);

	    if(v[1].compare("list") == 0) {
		handleImageListRequest(req, resp);
	    }
	}

	void handleImageListRequest(HTTPServerRequest &req, HTTPServerResponse &resp);

    private:
	static int count;
};

class MyRequestHandler : public HTTPRequestHandler
{
    public:
	virtual void handleRequest(HTTPServerRequest &req, HTTPServerResponse &resp)
	{
	    resp.setStatus(HTTPResponse::HTTP_OK);

	    vector<string> v;
	    URI u = URI(req.getURI());
	    u.getPathSegments(v);
	   
	    HTTPRequestHandler *handler;
	    if(v[0].compare("vm") == 0)
	    {
		handler = new VirtualMachineRequestHandler;
	    }
	    else if(v[0].compare("pm") == 0)
	    {
		handler = new ResourceServiceRequestHandler;
	    }
	    else
	    {
		handler = new ImageServiceRequestHandler;
	    }

	    handler->handleRequest(req, resp);
	}
};
