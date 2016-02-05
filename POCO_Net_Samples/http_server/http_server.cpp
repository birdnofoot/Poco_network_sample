// ��ó: http://codezine.jp/article/download/1064
// ���� �� ����

#include "Poco/Net/HTTPServer.h"
#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Net/HTTPRequestHandlerFactory.h"
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Net/ServerSocket.h"
#include "Poco/Util/ServerApplication.h"

#include "Poco/File.h"

class NotFileHandler : public Poco::Net::HTTPRequestHandler
{
public:

	void handleRequest(Poco::Net::HTTPServerRequest& request,
		Poco::Net::HTTPServerResponse& response)
	{
		response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_NOT_FOUND);
		response.setContentType("text/html");

		std::ostream& ostr = response.send();
		ostr << "<html><head>";
		ostr << "<title>MiniWebServer powered by POCO</title>";
		ostr << "</head><body>Error. No Fill<br />";
		ostr << "</body></html>";
	}
};

class FileRequestHandler : public Poco::Net::HTTPRequestHandler
{
public:

	void handleRequest(Poco::Net::HTTPServerRequest& request,
		Poco::Net::HTTPServerResponse& response)
	{
		try {
			std::string fpass = ".";
			fpass += request.getURI();

			response.sendFile(fpass, "text/html");
		}
		catch (Poco::Exception&)
		{
		}
	}
};

class MiniRequestHandlerFactory : public Poco::Net::HTTPRequestHandlerFactory
{
public:

	Poco::Net::HTTPRequestHandler*
		createRequestHandler(const Poco::Net::HTTPServerRequest& request)
	{
		std::string fpass = ".";
		fpass += request.getURI();

		Poco::File f(fpass);
		if (!f.exists() || !f.isFile()) return new NotFileHandler();

		return new FileRequestHandler();
	}
};

int main()
{
	// Ŭ���̾�Ʈ�� �ִ� ���� ������ ��
	int maxThreads = 1;

	// ���� ������ �� ����
	Poco::ThreadPool::defaultPool().addCapacity(maxThreads);

	// ���� �� Ŭ����
	Poco::Net::HTTPServerParams* pParams = new Poco::Net::HTTPServerParams;

	// ���� �䱸 ���� �ִ� ��
	pParams->setMaxQueued(100);
	pParams->setMaxThreads(maxThreads);
	// ������ port�� 9980 
	Poco::Net::ServerSocket svs(9980);

	// HTTP ���� Ŭ���� �ν��Ͻ�
	Poco::Net::HTTPServer    srv(new MiniRequestHandlerFactory(), svs, pParams);

	// ���� ����
	srv.start();

	getchar();

	// ���� ���� �ߴ�
	srv.stop();

	return 0;
}