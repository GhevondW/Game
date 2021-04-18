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

		int GetRow() const { 
			if (_config == nullptr) throw "invalid operation";
			return _config->row;  
		}

		int GetColumn() const { 
			if (_config == nullptr) throw "invalid operation";
			return _config->column; 
		}

		int GetMovesCount() const { 
			if (_config == nullptr) throw "invalid operation";
			return _config->moves_count; 
		}

		int GetFigColorsCount() const { 
			if (_config == nullptr) throw "invalid operation";
			return _config->fig_colors_count; 
		}

		const std::string& GetBoard() const {
			if (_config == nullptr) throw "invalid operation";
			return _config->board;
		}

		const std::vector<Objective>& GetObjectives() const { 
			if (_config == nullptr) throw "invalid operation";
			return _config->objectives; 
		}

	protected:
		std::unique_ptr<Config> _config{nullptr};
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

