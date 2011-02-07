/**********************************************************\
Copyright Mital Vora <mital.d.vora@gmail.com> 
\**********************************************************/

#ifndef TH_NETWORK_API_H
#define TH_NETWORK_API_H

#include <JSAPIAuto.h>

namespace th
{

///<summary>
/// This class represents the Network Interface in Thorium. 
///</summary>
class NetworkAPI: public FB::JSAPIAuto
{
	public:
		static const boost::shared_ptr<NetworkAPI> getInstance()
		{ return boost::shared_ptr<NetworkAPI>(new NetworkAPI()); }

		//FB::JSAPIPtr getTCPSocket(const std::string & host, const std::string & port);

	protected:
		NetworkAPI();
};

}
#endif // TH_NETWORK_API_H
