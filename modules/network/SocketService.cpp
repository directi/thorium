/**
* @author: Mital Vora <mital.d.vora@gmail.com>
*/

#include "SocketService.h"

namespace th
{
	std::auto_ptr<boost::asio::io_service> SocketService::io_service;

	std::auto_ptr<boost::asio::io_service::work> SocketService::io_idlework;

	std::auto_ptr<boost::thread> SocketService::io_thread;

	void SocketService::initialize()
	{
		SocketService::io_service.reset(new boost::asio::io_service());
		SocketService::io_idlework.reset(new boost::asio::io_service::work(*io_service));
		SocketService::io_thread.reset(new boost::thread(
			boost::bind(&boost::asio::io_service::run, SocketService::io_service.get())));
	}
	
	void SocketService::uninitialize()
	{
		io_service->stop();
		io_thread->join();
		io_idlework.reset();
		io_thread.reset();
		io_service.reset();
	}
}
