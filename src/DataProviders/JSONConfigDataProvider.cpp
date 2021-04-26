#include "ConfigDataProvider.h"
#include <nlohmann/json.hpp>
#include <fstream>

namespace game{
	static void from_json(const nlohmann::json& j, Objective& sb)
	{
		sb.code = j.at("code");
		sb.count = j.at("count");
	}

	static bool check_config(const Config& config)
	{
		auto res =  config.column >= 1 && config.column <= 10 && config.row >= 1 && config.row <= 10 &&
			config.moves_count > 0 && config.objectives.size() >= 1
			&& config.objectives.size() <= 3 && config.fig_colors_count > 0 && config.fig_colors_count <= 5;
		for (auto v : config.objectives)
		{
			if (v.count == 0) {
				return false;
			}
		}
		return res;
	}

	LoadStatus JSONConfigDataProvider::Load(const std::string& path)
	{
		using namespace nlohmann;
		LoadStatus status = LoadStatus::OK;
		//if (_config) return LoadStatus::OK;

		_config = std::make_unique<Config>();

		std::ifstream file{ path };
		if (file.is_open()) {
			json j = json::parse(file);
			j.at("row").get_to(_config->row);
			j.at("column").get_to(_config->column);
			j.at("moves_count").get_to(_config->moves_count);
			j.at("fig_colors_count").get_to(_config->fig_colors_count);
			j.at("board").get_to(_config->board);

			const json& sj = j.at("objectives");
			_config->objectives.resize(sj.size());
			std::copy(sj.begin(), sj.end(), _config->objectives.begin());
			if (!check_config(*_config)) {
				status = LoadStatus::Other;
			}
		}
		else
		{
			status = LoadStatus::FileNotFound;
		}

		file.close();
		return status;
	}
}