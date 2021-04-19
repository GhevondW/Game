#include "ConfigDataProvider.h"
#include <nlohmann/json.hpp>
#include <fstream>

namespace game{
	static void from_json(const nlohmann::json& j, Objective& sb)
	{
		sb.code = j.at("code");
		sb.count = j.at("count");
	}


	const Config& JSONConfigDataProvider::Load(const std::string& path)
	{
		using namespace nlohmann;
		if (_config) return *_config;

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
		}

		file.close();
		return *_config;
	}
}