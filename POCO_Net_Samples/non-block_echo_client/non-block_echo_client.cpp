
// Timespan�� ����ũ�μ�����, Timespan(long seconds, long microseconds); ����ϱ�
// �������� Ŭ���̾�Ʈ���� ������ ������ �Ϻη� 3�� ���Ŀ� ������.
// Ŭ���̾�Ʈ�� poll���� 2�ʷ� ��Ƽ� ��ȯ ���� ��� �������� ����. ���� ��¥ ���� ���� true�� ���� ��
// ���� ��Ȳ�� �°� ������ �����ؾ� �Ѵ�.

//
//EchoServer echoServer;
//StreamSocket ss;
//ss.connect(SocketAddress("localhost", echoServer.port()));
//ss.setBlocking(false);
//
//Timespan timeout(1000000);
//assert(ss.poll(timeout, Socket::SELECT_WRITE));
//int n = ss.sendBytes("hello", 5);
//assert(n == 5);
//
//char buffer[256];
//assert(ss.poll(timeout, Socket::SELECT_READ));
//n = ss.receiveBytes(buffer, sizeof(buffer));
//assert(n == 5);
//assert(std::string(buffer, n) == "hello");
//ss.close();