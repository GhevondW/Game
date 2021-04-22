#ifndef _MESSAGE_H_
#define _MESSAGE_H_

#include "IDrawable.h"
#include "GlobalDefines.h"

namespace game
{
	class AlertMessage : public IDrawable
	{
	public:
		AlertMessage(const std::string& text = "", size_t size = 30) : _text(text) , _size(size)
		{
			std::string path = R_PATH;
			_font.loadFromFile(path + "font.ttf");
		}
		~AlertMessage() = default;
		
		void Draw(sf::RenderWindow& window) override
		{
			auto size = window.getSize();
			sf::Text text;

			text.setString(_text);
			text.setFont(_font);
			text.setCharacterSize(_size);
			text.setFillColor(sf::Color::Black);
			text.setStyle(sf::Text::Bold);
			

			auto current_rec = text.getGlobalBounds();
			int cw = current_rec.width;
			int ch = current_rec.height;
			int ow = size.x;
			int oh = size.y;
			if (cw < ow && ch < oh) {
				int dw = (ow - cw) / 2;
				int dh = (oh - ch) / 2;
				text.setPosition(dw, dh);
			}

			window.draw(text);
		}

	private:
		std::string _text{};
		sf::Font	_font{};
		size_t		_size{};
	};
}

#endif // !_MESSAGE_H_
