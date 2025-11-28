#pragma once

namespace color
{
	struct RGBColor
	{
		int r = 0;
		int g = 0;
		int b = 0;
		int a = 0;
	};

	static const int maxColors = 4;
	enum class ColorsName
	{
		DarkGreen,
		LightGreen,
		DarkGray,
		RedNapthol,
	};

	const RGBColor colors[maxColors] =
	{
		{0,100,0,255},
		{144,238,144,255},
		{58,102,58,255},
		{200,20,43,255},
	};
}

