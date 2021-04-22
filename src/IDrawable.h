#ifndef _I_DRAWABLE_H_
#define _I_DRAWABLE_H_

#include <SFML/Graphics.hpp>

namespace game
{
	struct IDrawable
	{
		IDrawable() = default;
		virtual ~IDrawable() = default;

		virtual void Draw(sf::RenderWindow&) = 0;

	};
}

#endif // !_I_DRAWABLE_H_
