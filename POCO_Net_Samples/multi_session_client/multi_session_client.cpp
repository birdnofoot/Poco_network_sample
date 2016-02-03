
#include "Poco/Net/Net.h"
#include "Poco/Net/ServerSocket.h"
#include "Poco/Net/StreamSocket.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/DateTime.h"
#include "Poco/Timespan.h"
#include "Poco/Thread.h"

const Poco::UInt16 PORT = 32452;

int main()
{
	Poco::DateTime now;
	char szClientName[256] = { 0, };
	sprintf_s(szClientName, 256 - 1, "(%d-%d)", now.second(), now.millisecond());
		
	std::cout << "clinet(" << szClientName << ") ������ ���� �õ�..." << std::endl;
	Poco::Net::StreamSocket ss;

	try
	{
		ss.connect(Poco::Net::SocketAddress("localhost", PORT));

		for (int i = 0; i < 7; ++i)
		{
			char szMessage[256] = { 0, };
			sprintf_s(szMessage, 256 - 1, "%d, Send Message From %s", i, szClientName);
			auto nMsgLen = (int)strnlen_s(szMessage, 256 - 1);

			ss.sendBytes(szMessage, nMsgLen);

			std::cout << "������ ���� �޽���: " << szMessage << std::endl;


			char buffer[256] = { 0, };
			auto len = ss.receiveBytes(buffer, sizeof(buffer));

			if (len <= 0)
			{
				std::cout << "������ ������ ���������ϴ�" << std::endl;
				break;
			}

			std::cout << "�����κ��� ���� �޽���: " << buffer << std::endl;

			Poco::Thread::sleep(256);
		}

		ss.close();
	}
	catch (Poco::Exception& exc)
	{
		std::cout << "���� ���� ����: " << exc.displayText() << std::endl;
	}
		
	getchar();
	return 0;
}