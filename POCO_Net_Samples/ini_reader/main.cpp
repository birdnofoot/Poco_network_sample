#include <iostream>

#include "Poco/Util/IniFileConfiguration.h"

int main()
{
	try
	{
		Poco::AutoPtr<Poco::Util::IniFileConfiguration> pZoneServerConf(new Poco::Util::IniFileConfiguration("test.ini"));
		auto nMaxChannelCount = pZoneServerConf->getInt("ZoneServer.CHANNEL_COUNT");
		auto nMaxChannelUserCount = pZoneServerConf->getInt("ZoneServer.CHANNEL_USER_COUNT");

		std::cout << "nMaxChannelCount: " << nMaxChannelCount << std::endl;
		std::cout << "nMaxChannelUserCount: " << nMaxChannelUserCount << std::endl;

		return 0;
	}
	catch (Poco::FileNotFoundException e) // ������ ���� ���
	{
		std::cout << "error: " << e.displayText() << std::endl;
		return 0;
	}
	catch (Poco::NotFoundException& e)    // ���� Ű���尡 ���� ���
	{
		std::cout << "error: " << e.displayText() << std::endl;
		return 0;
	}
}