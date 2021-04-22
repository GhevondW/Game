#ifndef _ELEMENT_H_
#define _ELEMENT_H_

#include <SFML/Graphics.hpp>
#include "../DataProviders/ResourceDataProvider.h"
#include <string>
#include <memory>
#include <map>
#include "../GlobalDefines.h"

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
			HBOMB = 7,
			EMPTY = 8
		};

	public:
		Element() = default;
		Element(TYPE type,/*, const std::string code,*/ const sf::Texture& texture)
			:_type(type),
//			_code(code),
			_sprite(texture)
		{}

		sf::Sprite& GetSprite() { return _sprite; }
		void SetPosition(int x, int y) { _sprite.setPosition(x, y); }
		sf::FloatRect GetRectPosition() const { return _sprite.getGlobalBounds(); }
		void SetCenterOfRect(sf::FloatRect position)
		{
			auto current_rec = GetRectPosition();
			int cw = current_rec.width;
			int ch = current_rec.height;
			int ow = position.width;
			int oh = position.height;
			if (cw < ow && ch < oh) {
				int dw = (ow - cw) / 2;
				int dh = (oh - ch) / 2;
				_sprite.setPosition(position.left + dw, position.top + dh);
			}
		}
		TYPE GetType() const { return _type; }
//		const std::string& GetCode() const { return _code; }

	private:
		TYPE			_type{};
//		std::string		_code{};
		sf::Sprite		_sprite{};
	};

	class Tile
	{
	public:

		enum TYPE
		{
			ONE = 0,
			TWO = 1
		};

		Tile() = default;
		Tile(TYPE type, const std::string code, const sf::Texture& texture)
			:_type(type),
			_code(code),
			_sprite(texture)
		{}
		~Tile() = default;

		sf::Sprite& GetSprite() { return _sprite; }
		void SetPosition(int x, int y) { _sprite.setPosition(x, y); }
		sf::FloatRect GetRectPosition() const { return _sprite.getGlobalBounds(); }
		void SetCenterOfRect(sf::FloatRect position)
		{
			auto current_rec = GetRectPosition();
			float cw = current_rec.width;
			float ch = current_rec.height;
			float ow = position.width;
			float oh = position.height;
			if (cw < ow && ch < oh) {
				float dw = (ow - cw) / 2;
				float dh = (oh - ch) / 2;
				_sprite.setPosition(position.left + dw, position.top + dh);
			}
		}

		TYPE GetType() const { return _type; }
		const std::string& GetCode() const { return _code; }

	private:
		TYPE			_type{};
		std::string		_code{};
		sf::Sprite		_sprite{};
	};

	struct IElementFactory
	{
		IElementFactory() = default;
		virtual ~IElementFactory() = default;
		virtual Element::TYPE GetElemTypeByCode(const std::string& code) = 0;
		virtual Element* CreateElement(Element::TYPE type) = 0;
		virtual Element* CreateRandomElement(size_t max_degree) = 0;
		virtual Tile* CreateTile(Tile::TYPE type) = 0;
	};

	class ElementFactory : public IElementFactory
	{
	public:
		ElementFactory(std::shared_ptr<IResourceDataProvider> dp)
			:_resource_dp(dp)
		{
			//if (_resource_dp != nullptr) {
			//	std::string path = R_PATH;
			//	_resource_dp->Load(path + "res.json");
			//}
		}
		~ElementFactory() = default;

		Element::TYPE GetElemTypeByCode(const std::string& code)
		{
			const auto& ref = _resource_dp->GetElements();
			auto pos = std::find_if(std::begin(ref), std::end(ref), [&](const ElementInfo& info) -> bool
				{
					return info.code == code;
				});
			if (pos == end(ref)) return Element::EMPTY;
			int dist = std::distance(begin(ref), pos);
			return (Element::TYPE)dist;
		}

		Element* CreateElement(Element::TYPE type)
		{
			if (_resource_dp == nullptr) return nullptr;
			return new Element(type, _GetTexture(type));
		}
        
        Element* CreateRandomElement(size_t max_degree)
        {
            if (_resource_dp == nullptr || max_degree > 4) return nullptr;
            size_t rand = std::rand() % max_degree;
            Element::TYPE type = (Element::TYPE)rand;
            return new Element(type, _GetTexture(type));
        }

		Tile* CreateTile(Tile::TYPE type)
		{
			if (_resource_dp == nullptr) return nullptr;

			const auto& ref = _resource_dp->GetTiles();
			const TileInfo& info = ref[(int)type];

			if (_tile_texture_cache.count(type) == 0) {
				_tile_texture_cache.insert({ type, sf::Texture{} });
				std::string path = R_PATH;
				path += info.image_path;

				_tile_texture_cache[type].loadFromFile(path);
			}

			return new Tile(type, info.code, _tile_texture_cache.at(type));
		}

    private:
        
        sf::Texture& _GetTexture(Element::TYPE type)
        {
            if (_elem_texture_cache.count(type) == 0) {
                _elem_texture_cache.insert({ type, sf::Texture{} });
            }

            if (type != Element::TYPE::EMPTY) {
                const auto& ref = _resource_dp->GetElements();
                const ElementInfo& info = ref[(int)type];
                std::string path = R_PATH;
                path += info.image_path;

                _elem_texture_cache[type].loadFromFile(path);
            }
            
            return _elem_texture_cache[type];
        }
        
	public:
		std::shared_ptr<IResourceDataProvider>	_resource_dp{nullptr};
		std::map<Element::TYPE, sf::Texture>	_elem_texture_cache{};
		std::map<Tile::TYPE, sf::Texture>		_tile_texture_cache{};
	};
}

#endif // !_ELEMENT_H_
