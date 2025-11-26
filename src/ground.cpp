#include "ground.h"

#include "LLM_math.h"

#include "globals.h"
namespace rectangleTemp
{
	struct Rectangle
	{
		vec::Vector2 pos = { 500.f,0.f };
		float width = 200.f;
		float height = 55.f;
	};

	Rectangle rect;

	static void update()
	{
		rect.pos.y += 100 * externs::deltaT;
	}

	static void draw(sf::RenderWindow& window)
	{
		sf::RectangleShape rectangle({ rect.width, rect.height });

		rectangle.setPosition({ rect.pos.x, rect.pos.y });
		window.draw(rectangle);
	}
}

namespace ground
{
	static void drawPart(Part part, sf::RenderWindow& window);
	static void movePart(Part& part);
}

namespace ground
{
	static const float partDistance = 1500.f; //Screen width si point 0 esta en 0

	static const vec::Vector2 preset1[] =
	{
		{100,500},
		{1500,500},
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

		newGround.leftPart.shape = irregular::init(preset1, 2);
		newGround.leftPart.canMove = true;

		newGround.middlePart.shape = irregular::init(preset1, 2);
		newGround.middlePart.canMove = false;

		newGround.rightPart.shape = irregular::init(preset1, 2);
		newGround.rightPart.canMove = false;

		newGround.isLooping = false;

		return newGround;
	}

	void update(Ground& ground)
	{
		
		struct Player //placeholder
		{
			vec::Vector2 pos;
		};

		Player player;

		if (player.pos.x >= ground.rightPart.shape.points[0].x)
		{
			if (ground.leftPart.canMove)
			{
				movePart(ground.leftPart);
				ground.leftPart.canMove = false;
				ground.middlePart.canMove = true;
			}

			if (!ground.isLooping)
			{
				ground.isLooping = true;
			}
		}

		if (ground.isLooping)
		{
			if (player.pos.x >= ground.leftPart.shape.points[0].x)
			{
				if (ground.middlePart.canMove)
				{
					movePart(ground.middlePart);
					ground.middlePart.canMove = false;
					ground.rightPart.canMove = true;
				}
			}
			if (player.pos.x >= ground.middlePart.shape.points[0].x)
			{
				if (ground.rightPart.canMove)
				{
					movePart(ground.rightPart);
					ground.rightPart.canMove = false;
					ground.leftPart.canMove = true;
				}
			}
		}
		
		//Temp
		rectangleTemp::update();
	}

	void draw(Ground& ground, sf::RenderWindow& window)
	{
		drawPart(ground.leftPart, window);
		drawPart(ground.middlePart, window);
		drawPart(ground.rightPart, window);

		//Temp
		rectangleTemp::draw(window);
	}

	static void drawPart(Part part, sf::RenderWindow& window)
	{
		for (int i = 0; i < part.shape.pointAmount - 1; i++)
		{
			sf::VertexArray linesTemp = sf::VertexArray(sf::PrimitiveType::Lines, 2);

			linesTemp[0].position.x = part.shape.points[i].x;
			linesTemp[0].position.y = part.shape.points[i].y;

			linesTemp[1].position.x = part.shape.points[i + 1].x;
			linesTemp[1].position.y = part.shape.points[i + 1].y;

			window.draw(linesTemp);
		}
	}

	static void movePart(Part& part)
	{
		for (int i = 0; i < part.shape.pointAmount; i++)
		{
			part.shape.points[i].x += partDistance * totalParts - 1;
		}
	}
}