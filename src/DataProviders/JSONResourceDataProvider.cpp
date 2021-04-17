#include "ResourceDataProvider.h"
#include <nlohmann/json.hpp>
#include <fstream>

namespace game {
	static void from_json(const nlohmann::json& j, Color& sb)
	{
		sb.name = j.at("name");
		sb.image_path = j.at("image_path");
		sb.code = j.at("code");
	}

	static void from_json(const nlohmann::json& j, Bomb& sb)
	{
		sb.name = j.at("name");
		sb.image_path = j.at("image_path");
		sb.code = j.at("code");
	}

	static void from_json(const nlohmann::json& j, Tile& sb)
	{
		sb.name = j.at("name");
		sb.image_path = j.at("image_path");
		sb.code = j.at("code");
	}


	Resource JSONResourceDataProvider::ReadResource(const std::string& path)
	{
		using namespace nlohmann;

		Resource res;

		std::ifstream file{ path };

		if (file.is_open()) {
			json j = json::parse(file);

			const json& c = j.at("colors");
			res.colors.resize(c.size());
			std::copy(c.begin(), c.end(), res.colors.begin());

			const json& b = j.at("bombs");
			res.bombs.resize(b.size());
			std::copy(b.begin(), b.end(), res.bombs.begin());

			const json& t = j.at("tiles");
			res.tiles.resize(t.size());
			std::copy(t.begin(), t.end(), res.tiles.begin());
		}

		file.close();
		return res;
	}
}