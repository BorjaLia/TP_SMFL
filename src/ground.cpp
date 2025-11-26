#include "ground.h"

#include "LLM_math.h"

namespace ground
{
	static void drawPart(irregular::Irregular part, sf::RenderWindow& window);
}

namespace ground
{
	static const float partDistance = 1000.f;

	static const vec::Vector2 preset1[] =
	{
		{1,1},
		{100,100},
		{100,200},
	};

	static const vec::Vector2 preset2[] =
	{
		{3,5},
		{4,9},
		{5,10},
		{8,8}
	};

	static const vec::Vector2 preset3[] =
	{
		{4,3},
		{6,5},
		{1,19},
		{6,2}
	};

	static const vec::Vector2 preset4[] =
	{
		{1,1},
		{2,9},
		{3,10},
		{4,8}
	};

	const vec::Vector2 preset5[] =
	{
		{1,1},
		{2,9},
		{3,10},
		{4,8}
	};

	Ground init()
	{
		Ground newGround;

		newGround.leftPart = irregular::init(preset1, 3);
		newGround.middlePart = irregular::init(preset1, 3);
		newGround.rightPart = irregular::init(preset1, 3);

		return newGround;
	}

	void update(Ground& ground)
	{
		ground;
	}

	void draw(Ground& ground, sf::RenderWindow& window)
	{
		drawPart(ground.leftPart, window);
		drawPart(ground.middlePart, window);
		drawPart(ground.rightPart, window);
	}

	static void drawPart(irregular::Irregular part, sf::RenderWindow& window)
	{
		for (int i = 0; i < part.pointAmount - 1; i++)
		{
			sf::VertexArray linesTemp = sf::VertexArray(sf::PrimitiveType::Lines, 2);

			linesTemp[0].position.x = part.points[i].x;
			linesTemp[0].position.y = part.points[i].y;

			linesTemp[1].position.x = part.points[i + 1].x;
			linesTemp[1].position.y = part.points[i + 1].y;

			window.draw(linesTemp);
		}
	}
}