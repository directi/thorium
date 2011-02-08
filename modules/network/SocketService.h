/**
* @author: Mital Vora <mital.d.vora@gmail.com>
*/

#ifndef _SOCKET_SERVICE_H_
#define _SOCKET_SERVICE_H_

#include <string>
#include <deque>

#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>

using boost::asio::ip::tcp;

#include <boost/bind.hpp>

namespace ti
{
	class SocketService
	{
	public:
		static void initialize();
		static void uninitialize();
		static boost::asio::io_service* getIOService()
		{
			return io_service.get();
		}

	private:
		static std::auto_ptr<boost::asio::io_service> io_service;
		static std::auto_ptr<boost::asio::io_service::work> io_idlework;
		static std::auto_ptr<boost::thread> io_thread;
	};
}
#endif
