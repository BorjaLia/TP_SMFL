#include "color.h"

namespace color
{
	sf::Color toSFMLColor(color::RGBColor color)
	{
		return sf::Color(static_cast<uint8_t>(color.r), static_cast<uint8_t>(color.g), static_cast<uint8_t>(color.b));
	}
}