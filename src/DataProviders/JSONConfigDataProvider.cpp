#include "ConfigDataProvider.h"
#include <nlohmann/json.hpp>
#include <fstream>

namespace game{
	static void from_json(const nlohmann::json& j, Objective& sb)
	{
		sb.color = j.at("color");
		sb.count = j.at("count");
	}


	Config JSONConfigDataProvider::ReadConfig(const std::string& path)
	{
		using namespace nlohmann;
		Config conf;
		std::ifstream file{ path };
		if (file.is_open()) {
			json j = json::parse(file);

			j.at("row").get_to(conf.row);
			j.at("column").get_to(conf.column);
			j.at("moves_count").get_to(conf.moves_count);
			j.at("fig_colors_count").get_to(conf.fig_colors_count);
			j.at("board").get_to(conf.board);

			const json& sj = j.at("objectives");
			conf.objectives.resize(sj.size());
			std::copy(sj.begin(), sj.end(), conf.objectives.begin());
		}
		file.close();
		return conf;
	}
}