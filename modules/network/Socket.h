/**
* @author: Mital Vora <mital.d.vora@gmail.com>
*/

#ifndef _SOCKET_UTILS_H_
#define _SOCKET_UTILS_H_

#include <JSAPIAuto.h>
#include <variant_list.h>

#include "SocketService.h"
#include "SocketExceptions.h"

#include <string>
#include <deque>

#define BUFFER_SIZE 1024   // choose a reasonable size to send back to JS

namespace th
{
	template <class T>
	class Socket
	: public FB::JSAPIAuto
	{
	public:
		Socket<T>(const std::string & name);
		virtual ~Socket();

	protected:
		T *socket;

		boost::mutex write_mutex;
		std::deque<std::string> write_buffer;
		char read_data_buffer[BUFFER_SIZE + 1];
		bool non_blocking;
		enum SOCK_STATE_en { SOCK_CLOSED,
			SOCK_CONNECTING,
			SOCK_CONNECTED,
			SOCK_HANDSHAKE_IN_PROGRESS,
			SOCK_CLOSING
		} sock_state;

		void on_read(char * data, int size);
		void on_error(const std::string& error_text);
		void on_close();

		void registerHandleRead();
		virtual bool CompleteClose()=0;

		template<typename> friend class Socket;
		template <class T1>
		void copyHandlers(Socket<T1> *b)
		{
			this->onRead = b->onRead;
			this->onError = b->onError;
			this->onClose = b->onClose;
		}

	private:

		bool write(const std::string &data);
		std::string read();
		bool close();
		bool isClosed();

		void registerHandleWrite();
		void handleWrite(const boost::system::error_code& error, std::size_t bytes_transferred);
		void writeAsync(const std::string &data);
		bool writeSync(const std::string &data);

		void handleRead(const boost::system::error_code& error, std::size_t bytes_transferred);
	};


	template <class T>
	Socket<T>::Socket(const std::string & name)
	: socket(NULL),
	non_blocking(false),
	sock_state(SOCK_CLOSED)
	{
		registerMethod("read", make_method(this, &Socket::read));
		registerMethod("write", make_method(this, &Socket::write));
		registerMethod("isClosed", make_method(this, &Socket::isClosed));
		registerMethod("close", make_method(this, &Socket::close));
		
		registerEvent("onRead");
		registerEvent("onError");
		registerEvent("onClose");
		
	}

	template <class T>
	Socket<T>::~Socket()
	{
		if (socket)
		{
			delete socket;
			socket = NULL;
		}
	}


	template <class T>
	void Socket<T>::on_read(char * data, int size)
	{
		this->FireEvent("onRead", FB::variant_list_of(std::string(data, size)));
		//GetLogger()->Warn("Socket::onRead: not read subscriber registered:  " + string(data, size));
	}

	template <class T>
	void Socket<T>::on_error(const std::string& error_text)
	{
			this->FireEvent("onError", FB::variant_list_of(error_text));
	}

	template <class T>
	void Socket<T>::on_close()
	{
		this->FireEvent("onClose", FB::variant_list_of());
	}

	template <class T>
	bool Socket<T>::close()
	{
		return this->CompleteClose();
	}

	template <class T>
	bool Socket<T>::isClosed()
	{
		return (this->sock_state == SOCK_CLOSED);
	}

	

	template <class T>
	void Socket<T>::registerHandleWrite()
	{
		boost::asio::async_write(*socket,
			boost::asio::buffer(write_buffer.front().c_str(), write_buffer.front().size()),
			boost::bind(&Socket::handleWrite, this,
			boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));

	}

	template <class T>
	void Socket<T>::handleWrite(const boost::system::error_code& error, std::size_t bytes_transferred)
	{
		if (error)
		{
			if (error == boost::asio::error::operation_aborted)
			{
				//GetLogger()->Warn("Socket::handleWrite: operation aborted.");
				return;
			}
			this->on_error(error.message());
			return;
		}
		boost::mutex::scoped_lock lock(write_mutex);
		write_buffer.pop_front();
		if (!write_buffer.empty())
		{
			this->registerHandleWrite();
		}
	}

	template <class T>
	void Socket<T>::writeAsync(const std::string &data)
	{
		boost::mutex::scoped_lock lock(write_mutex);
		bool write_in_progress = !write_buffer.empty();
		write_buffer.push_back(data);
		if (!write_in_progress)
		{
			this->registerHandleWrite();
		}
	}

	template <class T>
	bool Socket<T>::writeSync(const std::string &data)
	{
		try
		{
			boost::asio::write(*socket, boost::asio::buffer(data.c_str(), data.size()));
		}
		catch(std::exception& e)
		{
			this->CompleteClose();
			this->on_error(e.what());
			return false;
		}
		return true;
	}

	template <class T>
	bool Socket<T>::write(const std::string &data)
	{
		try
		{
			if (this->sock_state != SOCK_CONNECTED)
			{
				throw TCPSocketWriteException();
			}
			if(non_blocking)
			{
				writeAsync(data);
				return true;
			}
			return writeSync(data);
		}
		catch(SocketException &e)
		{
			throw FB::script_error(e.what());
		}
	}

	template <class T>
	void Socket<T>::handleRead(const boost::system::error_code& error, std::size_t bytes_transferred)
	{
		if (error)
		{
			if (error == boost::asio::error::operation_aborted)
			{
				//GetLogger()->Warn("Socket::handleRead: operation aborted.");
				return;
			}
			this->on_error(error.message());
			return;
		}
		this->on_read(read_data_buffer, bytes_transferred);
		this->registerHandleRead();
	}


	template <class T>
	void Socket<T>::registerHandleRead()
	{
		boost::asio::async_read(*socket,
			boost::asio::buffer(read_data_buffer, BUFFER_SIZE),
			boost::asio::transfer_at_least(1),
			boost::bind(&Socket::handleRead, this,
			boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
	}

	template <class T>
	std::string Socket<T>::read()
	{
		try
		{
			if (this->sock_state != SOCK_CONNECTED)
			{
				throw TCPSocketReadNotOpenException();
			}
			// TODO: implement sync read
			size_t size = 0;
			try
			{
				size = boost::asio::read(*socket, boost::asio::buffer(read_data_buffer, BUFFER_SIZE),
										 boost::asio::transfer_at_least(1));
			}
			catch(std::exception & e)
			{
				this->CompleteClose();
				this->on_error(e.what());
				throw TCPSocketReadException();
			}
			if (size > 0)
			{
				return std::string(read_data_buffer, size);
			}
			return std::string("");
		}
		catch(SocketException &e)
		{
			throw FB::script_error(e.what());
		}
	}

}
#endif
