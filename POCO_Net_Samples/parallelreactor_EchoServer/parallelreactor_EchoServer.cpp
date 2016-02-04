// ����: http://www.cnblogs.com/Leo-Forest/archive/2013/05/16/3082218.html
#include <iostream>
//#include <cstdlib>

#include <Poco/Net/SocketReactor.h>
#include <Poco/Net/SocketNotification.h>
#include <Poco/Net/SocketConnector.h>
#include <Poco/Net/ParallelSocketAcceptor.h>
#include <Poco/Net/SocketAcceptor.h>
#include <Poco/Net/StreamSocket.h>
#include <Poco/Net/ServerSocket.h>
#include <Poco/Net/SocketAddress.h>
#include <Poco/Observer.h>
#include <Poco/Thread.h>

const Poco::UInt16 PORT = 32452;

class Session {
public:
	Session(Poco::Net::StreamSocket& socket, Poco::Net::SocketReactor& reactor) :
		m_socket(socket),
		m_reactor(reactor)
	{
		m_peerAddress = socket.peerAddress().toString();
		std::cout << "connection from " << m_peerAddress << " ..." << std::endl;
		m_reactor.addEventHandler(m_socket,
			Poco::Observer<Session, Poco::Net::ReadableNotification>(*this, &Session::onReadable));

		//Poco::Thread::sleep(10000); // ��Ƽ������ �ƴ�
	}

	~Session()
	{
		std::cout << m_peerAddress << " disconnected ..." << std::endl;
		m_reactor.removeEventHandler(m_socket,
			Poco::Observer<Session, Poco::Net::ReadableNotification>(*this, &Session::onReadable)
			);
	}

	void onReadable(Poco::Net::ReadableNotification* pNf)
	{
		pNf->release();

		char buffer[256] = { 0, };
		int n = m_socket.receiveBytes(buffer, sizeof(buffer));
		if (n > 0)
		{
			std::cout << "Ŭ���̾�Ʈ���� ���� �޽���: " << buffer << std::endl;

			char szSendMessage[256] = { 0, };
			sprintf_s(szSendMessage, 256 - 1, "Re:%s", buffer);
			int nMsgLen = (int)strnlen_s(szSendMessage, 256 - 1);

			m_socket.sendBytes(szSendMessage, nMsgLen);

			Poco::Thread::sleep(10000); // �����庰�� Ŭ���̾�Ʈ �Ҵ�ǰ�, ���� �ٸ� �����忡 ���ӵ� Ŭ���̾�Ʈ�� �������� �ʴ´�
		}
		else
		{
			m_socket.shutdownSend();
			delete this;  // �޸� �������� ������ �Ҹ��ڰ� ȣ����� �ʴ´�.
		}
	}


private:
	Poco::Net::StreamSocket   m_socket;
	std::string m_peerAddress;
	Poco::Net::SocketReactor& m_reactor;
};


int main()
{
	Poco::Net::ServerSocket serverSocket(PORT);
	Poco::Net::SocketReactor reactor;
	Poco::Net::ParallelSocketAcceptor<Session, Poco::Net::SocketReactor> acceptor(serverSocket, reactor);

	std::cout << "Parallel Reactor: " << "starting..." << std::endl;
	reactor.run();

	return 0;
}