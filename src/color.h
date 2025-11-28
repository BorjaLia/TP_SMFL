#pragma once

#include "SFML/Graphics.hpp"

namespace color
{
	struct RGBColor
	{
		int r = 0;
		int g = 0;
		int b = 0;
		int a = 0;
	};

	static const int maxColors = 7;
	enum class ColorsName
	{
		DarkGreen,
		LightGreen,
		DarkGray,
		RedNapthol,

		SlightDarkWhite,
		MediumWhite,
		DarkWhite,
	};

	const RGBColor colors[maxColors] =
	{
		{0,100,0,255},
		{144,238,144,255},
		{58,102,58,255},
		{200,20,43,255},

		{183,185,183,255},
		{170,172,171,255},
		{121,124,122,255},
	};

	sf::Color toSFMLColor(color::RGBColor color);
}

