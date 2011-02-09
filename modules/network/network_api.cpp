/**********************************************************\
Copyright Mital Vora <mital.d.vora@gmail.com> 
\**********************************************************/


#include "network_api.h"
#include "tcpsocket.h"



namespace th
{

	NetworkAPI::NetworkAPI()
	{
		SocketService::initialize();
		registerMethod("getTCPSocket", make_method(this, &NetworkAPI::getTCPSocket));
	}

	NetworkAPI::~NetworkAPI()
	{
		throw FB::script_error("Error network api destructor");
		SocketService::uninitialize();
	}

	FB::JSAPIPtr NetworkAPI::getTCPSocket(const std::string & host, const std::string & port)
	{
		return boost::shared_ptr<th::TCPSocket>(new TCPSocket(host, port));
	}
}
