/**
 * @author: Mital Vora <mital.d.vora@gmail.com>
 */

#ifndef _TCP_SOCKET_BINDING_H_
#define _TCP_SOCKET_BINDING_H_


#include "Socket.h"
#include "SocketExceptions.h"


namespace th
{
	class TCPSocket
		: public Socket<tcp::socket>
	{
	public:
		TCPSocket(const std::string & hostname,
						 const std::string& port);
		virtual ~TCPSocket();

		tcp::socket * resetSocket();

	private:

		bool connect(long timeout = 10);
		void connectNB();
		void setKeepAlives(bool keep_alive);
		void setKeepAliveTimes(int inactivetime, int resendtime);


		const std::string hostname;
		const std::string port;

		tcp::resolver resolver;
		void on_connect();

		void setKeepAlive(bool keep_alives);
		//void setKeepAliveTimes(int inactivetime, int resendtime);

		tcp::resolver::iterator resolveHost();
		bool tryConnect(tcp::resolver::iterator endpoint_iterator);

		void registerHandleResolve();

		void handleResolve(const boost::system::error_code& error, tcp::resolver::iterator endpoint_iterator);
		void registerHandleConnect(tcp::resolver::iterator endpoint_iterator);
		void handleConnect(const boost::system::error_code& error, tcp::resolver::iterator endpoint_iterator);

		virtual bool CompleteClose()
		{
			// Log  ->Debug("Closing socket to: %s:%d ", this->hostname.c_str(), this->port.c_str());
			if ((this->sock_state == SOCK_CONNECTED)
				|| (this->sock_state == SOCK_CONNECTING))
			{
				this->sock_state = SOCK_CLOSING;
				if (socket)
				{
					socket->close();
				}
				this->sock_state = SOCK_CLOSED;
				return true;
			}
			return false;
		}

	};
}

#endif
