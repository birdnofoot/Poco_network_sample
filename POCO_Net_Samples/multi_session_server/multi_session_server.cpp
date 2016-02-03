#include <vector>

#include "Poco/Net/Net.h"
#include "Poco/Net/ServerSocket.h"
#include "Poco/Net/StreamSocket.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/Timespan.h"

const Poco::UInt16 PORT = 32452;

int main()
{
	std::cout << "���� �ʱ�ȭ ����" << std::endl;

	Poco::Net::SocketAddress server_add(PORT);
	Poco::Net::ServerSocket server_sock(server_add);

	std::cout << "���� �ʱ�ȭ �Ϸ�. Ŭ���̾�Ʈ ���� ��� ��..." << std::endl;


	std::vector<Poco::Net::StreamSocket> sessions;
	
	while (true)
	{
		try
		{			
			while (true)
			{
				Poco::Net::Socket::SocketList reads, empties;
				reads.push_back(server_sock);

				for (auto& session : sessions)
				{
					reads.push_back(session);
				}

				server_sock.select(reads, empties, empties, Poco::Timespan());
				for (auto& session : reads)
				{
					if (session == server_sock)
					{
						auto peer = server_sock.acceptConnection();
						sessions.push_back(peer);
						std::cout << "client accepted" << std::endl;
					}
					else
					{
						Poco::Net::StreamSocket peer(session);

						const short BUF_SIZE = 128;
						char buffer[BUF_SIZE] = { 0, };
						int result = peer.receiveBytes(buffer, sizeof(buffer));
						if (result == 0)
						{							
							peer.close();
							auto it = std::remove(sessions.begin(), sessions.end(), peer);
							sessions.erase(it);

							std::cout << "cannot read, peer closed" << std::endl;
						}
						else
						{
							std::cout << "Ŭ���̾�Ʈ���� ���� �޽���: " << buffer << std::endl;

							char szSendMessage[256] = { 0, };
							sprintf_s(szSendMessage, 128 - 1, "Re:%s", buffer);
							int nMsgLen = (int)strnlen_s(szSendMessage, 256 - 1);
							peer.sendBytes(szSendMessage, nMsgLen);
						}
					}
				}
			}
		}
		catch (Poco::Exception& exc)
		{
			std::cerr << "EchoServer: " << exc.displayText() << std::endl;
		}
	}

	getchar();
	return 0;
}