/**
 * @author: Mital Vora <mital.d.vora@gmail.com>
 */

#include "tcpsocket.h"
#include "SocketService.h"

namespace th
{
	TCPSocket::TCPSocket(const std::string& hostname, const std::string& port) :
	Socket<tcp::socket>(std::string("Socket.TCPSocket")),
		hostname(hostname),
		port(port),
		resolver(*SocketService::getIOService())
	{
		registerEvent("onConnected");
		
		this->socket = new tcp::socket(*SocketService::getIOService());

		registerMethod("connect", make_method(this, &TCPSocket::connect));
		registerMethod("connectNB", make_method(this, &TCPSocket::connectNB));

		// Enables/disables keepalives.
		registerMethod("setDisconnectionNotifications", make_method(this, &TCPSocket::setKeepAlives));

		// Sets the timeouts for the keepalive times, should be called before connecting the socket.
		registerMethod("setDisconnectionNotificationTime", make_method(this, &TCPSocket::setKeepAliveTimes));
	}

	TCPSocket::~TCPSocket()
	{
		this->CompleteClose();
	}

	tcp::socket * TCPSocket::resetSocket()
	{
		tcp::socket * temp = this->socket;
		this->socket->cancel();
		this->socket = NULL;
		return temp;
	}



	void TCPSocket::setKeepAlives(bool keep_alive)
	{
		this->setKeepAlive(keep_alive);
	}

	void TCPSocket::setKeepAliveTimes(int inactivetime, int resendtime) 
	{
		// TODO:
		//if(this->sock_state != SOCK_CLOSED)
		//	throw FB::script_error("You can only set the keep-alive times before connecting the socket");

		//int inactivetime = args.at(0)->ToInt();
		//int resendtime = args.at(1)->ToInt();
		//this->setKeepAliveTimes(inactivetime, resendtime);
	}


	void TCPSocket::on_connect()
	{
		this->FireEvent("onConnect", FB::variant_list_of());
	}


	void TCPSocket::setKeepAlive(bool keep_alives)
	{
		boost::asio::socket_base::keep_alive option(keep_alives);
		if (socket)
		{
			socket->set_option(option);
		}
	}

	//void TCPSocket::setKeepAliveTimes(int inactivetime, int resendtime) 
	//{
	//	if(this->sock_state != SOCK_CLOSED) 
	//	{
	//		throw TCPSocketConnectedException();
	//	}
	//	//this->inactivetime = inactivetime;
	//	//this->resendtime = resendtime;
	//}

	tcp::resolver::iterator TCPSocket::resolveHost()
	{
		tcp::resolver::query query(hostname, port);
		return resolver.resolve(query);
	}

	bool TCPSocket::tryConnect(tcp::resolver::iterator endpoint_iterator)
	{
		try
		{
			if (socket)
			{
				socket->connect(*endpoint_iterator);
			}
		}
		catch(std::exception& e)
		{
			this->CompleteClose();
			return false;
		}
		return true;
	}

	bool TCPSocket::connect(long timeout)
	{
		try
		{
			//GetLogger()->Debug("Connecting Blocking.");
			if(this->sock_state != SOCK_CLOSED)
			{
				throw TCPSocketConnectedException();
			}
			non_blocking = false;
			this->sock_state = SOCK_CONNECTING;
			
			//TODO: implement timeout for connect
			tcp::resolver::iterator endpoint_iterator;
			try
			{
				endpoint_iterator = this->resolveHost();
			}
			catch(std::exception& e)
			{
				this->on_error(e.what());
				this->sock_state = SOCK_CLOSED;
				return false;
			}
			
			bool ret;
			while(endpoint_iterator != tcp::resolver::iterator())
			{
				ret = tryConnect(endpoint_iterator);
				if (ret)
					break;
				endpoint_iterator = ++endpoint_iterator;
			}
			this->sock_state = (ret)?SOCK_CONNECTED:SOCK_CLOSED;
			return ret;
		}
		catch(SocketException & e)
		{
			throw FB::script_error(e.what());
		}
		
	}


	void TCPSocket::connectNB()
	{
		try
		{
			//GetLogger()->Debug("Connecting non Blocking.");
			if(this->sock_state != SOCK_CLOSED)
			{
				throw TCPSocketConnectedException();
			}
			non_blocking = true;
			this->sock_state = SOCK_CONNECTING;
			this->registerHandleResolve();
		}
		catch(SocketException &e)
		{
			throw FB::script_error(e.what());
		}
	}

	void TCPSocket::registerHandleResolve()
	{
		tcp::resolver::query query(hostname, port);
		resolver.async_resolve(query,
			boost::bind(&TCPSocket::handleResolve, this,
			boost::asio::placeholders::error, boost::asio::placeholders::iterator));
	}

	void TCPSocket::handleResolve(const boost::system::error_code& error, tcp::resolver::iterator endpoint_iterator)
	{
		if (error)
		{
			if (error == boost::asio::error::operation_aborted)
			{
				//GetLogger()->Warn("Socket::handleResolve: operation aborted.");
				return;
			}
			this->on_error(error.message());
			return;
		}
		registerHandleConnect(endpoint_iterator);
	}

	void TCPSocket::registerHandleConnect(tcp::resolver::iterator endpoint_iterator)
	{
		if (endpoint_iterator != tcp::resolver::iterator() && socket)
		{
			socket->async_connect(*endpoint_iterator,
				boost::bind(&TCPSocket::handleConnect, this,
				boost::asio::placeholders::error, ++endpoint_iterator));
			return;
		}
		this->on_error("TCPSocket Host resolution Error");
	}

	void TCPSocket::handleConnect(const boost::system::error_code& error, tcp::resolver::iterator endpoint_iterator)
	{
		if (!error)
		{
			this->sock_state = SOCK_CONNECTED;
			this->on_connect();
			this->registerHandleRead();
			return;
		}

		this->CompleteClose();
		if (endpoint_iterator != tcp::resolver::iterator())
		{
			this->registerHandleConnect(endpoint_iterator);
			return;
		}

		if (error == boost::asio::error::operation_aborted)
		{
			//GetLogger()->Warn("Socket::handleWrite: operation aborted.");
			return;
		}

		this->on_error(error.message());
	}
}
