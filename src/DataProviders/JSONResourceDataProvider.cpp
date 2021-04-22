#include "ResourceDataProvider.h"
#include <nlohmann/json.hpp>
#include <fstream>

namespace game {
	static void from_json(const nlohmann::json& j, ElementInfo& sb)
	{
		sb.name = j.at("name");
		sb.image_path = j.at("image_path");
		sb.code = j.at("code");
	}

	static void from_json(const nlohmann::json& j, TileInfo& sb)
	{
		sb.name = j.at("name");
		sb.image_path = j.at("image_path");
		sb.code = j.at("code");
	}


	LoadStatus JSONResourceDataProvider::Load(const std::string& path)
	{
		using namespace nlohmann;

		//if (_resource) return *_resource;
		LoadStatus status = LoadStatus::OK;
		_resource = std::make_unique<Resource>();
		std::ifstream file{ path };

		if (file.is_open()) {
			json j = json::parse(file);

			const json& c = j.at("elements");
			_resource->elements.clear();
			_resource->elements.resize(c.size());
			std::copy(c.begin(), c.end(), _resource->elements.begin());

			const json& t = j.at("tiles");
			_resource->tiles.clear();
			_resource->tiles.resize(t.size());
			std::copy(t.begin(), t.end(), _resource->tiles.begin());
		}
		else {
			status = LoadStatus::FileNotFound;
		}

		file.close();
		return status;
	}
}