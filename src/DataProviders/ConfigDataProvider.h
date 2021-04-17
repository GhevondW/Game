#ifndef _CONFIG_DATA_PROVIDER_H_
#define _CONFIG_DATA_PROVIDER_H_

#include "Config.h"
#include <memory>

namespace game {

	struct IConfigDataProvider
	{
		IConfigDataProvider() = default;
		virtual ~IConfigDataProvider() = default;

		virtual const Config& Load(const std::string& path) = 0;

		int GetRow() const { return _config.row;  }
		int GetColumn() const { return _config.column; }
		int GetMovesCount() const { return _config.moves_count; }
		int GetFigColorsCount() const { return _config.fig_colors_count; }
		const std::vector<Objective>& GetObjectives() const { return _config.objectives; }
	protected:
		Config _config{};
	};

	struct JSONConfigDataProvider : public IConfigDataProvider
	{
		const Config& Load(const std::string& path) override;
	};

	//struct XMLConfigDataProvider : public IConfigDataProvider
	//{
	//	Config ReadConfig(const std::string& path) override;
	//};
}

#endif // !_CONFIG_DATA_PROVIDER_H_

