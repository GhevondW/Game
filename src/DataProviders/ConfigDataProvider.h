#ifndef _CONFIG_DATA_PROVIDER_H_
#define _CONFIG_DATA_PROVIDER_H_

#include "Config.h"
#include <memory>
#include "LoadStatus.h"

namespace game {

	struct IObjectiveProvider
	{
		IObjectiveProvider() = default;
		virtual ~IObjectiveProvider() = default;

		virtual const std::vector<Objective>& GetObjectives() const = 0;
        virtual size_t GetMovesCount() const = 0;
	};

	struct IConfigDataProvider : public IObjectiveProvider
	{
		IConfigDataProvider() = default;
		virtual ~IConfigDataProvider() = default;

		virtual LoadStatus Load(const std::string& path) = 0;

		size_t GetRow() const { 
			if (_config == nullptr) throw "invalid operation";
			return _config->row;  
		}

		size_t GetColumn() const {
			if (_config == nullptr) throw "invalid operation";
			return _config->column; 
		}

		size_t GetMovesCount() const {
			if (_config == nullptr) throw "invalid operation";
			return _config->moves_count; 
		}

		size_t GetFigColorsCount() const {
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
		LoadStatus Load(const std::string& path) override;
	};

	//struct XMLConfigDataProvider : public IConfigDataProvider
	//{
	//	Config ReadConfig(const std::string& path) override;
	//};
}

#endif // !_CONFIG_DATA_PROVIDER_H_

