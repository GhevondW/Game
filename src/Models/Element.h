#ifndef _ELEMENT_H_
#define _ELEMENT_H_

#include <SFML/Graphics.hpp>
#include "../DataProviders/ResourceDataProvider.h"
#include <string>
#include <memory>
#include <map>

#define R_PATH "C:\\Users\\Ghevond\\Desktop\\Game\\DevTestGame\\resources\\"

namespace game
{
	class Element
	{
	public:

		enum TYPE
		{
			RED = 0,
			GREEN = 1,
			BLUE = 2,
			VIOLET = 3,
			ORANGE = 4,
			BOMB = 5,
			VBOMB = 6,
			HBOMB = 7
		};

	public:
		Element() = default;
		Element(TYPE type, const std::string code, const sf::Texture& texture)
			:_type(type),
			_code(code),
			_sprite(texture)
		{}

		sf::Sprite& GetSprite() { return _sprite; }
		void SetPosition(int x, int y) { _sprite.setPosition(x, y); }
		TYPE GetType() const { return _type; }
		const std::string& GetCode() const { return _code; }

	private:
		TYPE			_type{};
		std::string		_code{};
		sf::Sprite		_sprite{};
	};

	class ElementFactory
	{
	public:
		ElementFactory(std::unique_ptr<IResourceDataProvider> dp)
			:_resource_dp(std::move(dp))
		{
			if (_resource_dp != nullptr) {
				std::string path = R_PATH;
				_resource_dp->Load(path + "res.json");
			}
		}
		~ElementFactory() = default;

		Element* CreateElement(Element::TYPE type)
		{
			const auto& ref = _resource_dp->GetElements();
			const ElementInfo& info = ref[(int)type];

			if (_texture_cache.count(type) == 0) {
				_texture_cache.insert({ type, sf::Texture{} });
				std::string path = R_PATH;
				path += info.image_path;

				_texture_cache[type].loadFromFile(path);
			}

			return new Element(type, info.code, _texture_cache.at(type));
		}

	public:
		std::unique_ptr<IResourceDataProvider>	_resource_dp{nullptr};
		std::map<Element::TYPE, sf::Texture>	_texture_cache{};
	};
}

#endif // !_ELEMENT_H_
