#ifndef _CONFIG_DATA_PROVIDER_H_
#define _CONFIG_DATA_PROVIDER_H_

#include "Config.h"

namespace game {

	struct IConfigDataProvider
	{
		virtual ~IConfigDataProvider() = default;
		virtual Config ReadConfig(const std::string& path) = 0;
	};

	struct JSONConfigDataProvider : public IConfigDataProvider
	{
		Config ReadConfig(const std::string& path) override;
	};

	//struct XMLConfigDataProvider : public IConfigDataProvider
	//{
	//	Config ReadConfig(const std::string& path) override;
	//};
}

#endif // !_CONFIG_DATA_PROVIDER_H_

