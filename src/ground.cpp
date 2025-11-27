#include "ground.h"

#include "LLM_math.h"

namespace ground
{
	static void drawPart(Part part, sf::RenderWindow& window);
	static void movePart(Part& part);
	static void initParts(Ground& ground);
	static float randomizedY(vec::Vector2 point);
}

namespace ground
{
	static const float distBetweenPoints = 10.f;
	static const int maxPoints = 1000;
	static const float partDistance = distBetweenPoints * maxPoints;
	static const float commonHeight = 500;

	static const float startY = 500.f;

	static const float randomRepeatAmount = 30.f;
	static const float randomScale1 = 5.f;
	static const float randomScale2 = 2.f;
	static const float randomPiScale = 0.4f;
	static const float factor1 = 5.4f;
	static const float randomVariationY = 0.4f;

	Ground init()
	{
		Ground newGround;

		initParts(newGround);

		newGround.isLooping = false;

		return newGround;
	}

	void update(Ground& ground, car::Car car)
	{
		if (car.transform.position.x >= ground.parts[static_cast<int>(PartName::Right)].shape.points[0].x)
		{
			if (ground.parts[static_cast<int>(PartName::Left)].canMove)
			{
				movePart(ground.parts[static_cast<int>(PartName::Left)]);
				ground.parts[static_cast<int>(PartName::Left)].canMove = false;
				ground.parts[static_cast<int>(PartName::Middle)].canMove = true;
			}

			if (!ground.isLooping)
			{
				ground.isLooping = true;
			}
		}

		if (ground.isLooping)
		{
			if (car.transform.position.x >= ground.parts[static_cast<int>(PartName::Left)].shape.points[0].x)
			{
				if (ground.parts[static_cast<int>(PartName::Middle)].canMove)
				{
					movePart(ground.parts[static_cast<int>(PartName::Middle)]);
					ground.parts[static_cast<int>(PartName::Middle)].canMove = false;
					ground.parts[static_cast<int>(PartName::Right)].canMove = true;
				}
			}
			if (car.transform.position.x >= ground.parts[static_cast<int>(PartName::Middle)].shape.points[0].x)
			{
				if (ground.parts[static_cast<int>(PartName::Right)].canMove)
				{
					movePart(ground.parts[static_cast<int>(PartName::Right)]);
					ground.parts[static_cast<int>(PartName::Right)].canMove = false;
					ground.parts[static_cast<int>(PartName::Left)].canMove = true;
				}
			}
		}
	}

	void draw(Ground& ground, sf::RenderWindow& window)
	{
		drawPart(ground.parts[static_cast<int>(PartName::Left)], window);
		drawPart(ground.parts[static_cast<int>(PartName::Middle)], window);
		drawPart(ground.parts[static_cast<int>(PartName::Right)], window);
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
		part.shape.points[0].x += partDistance * totalParts - 1;
	}

	static void initParts(Ground& ground)
	{
		vec::Vector2 pointsAux[maxPoints];

		for (int i = 0; i < totalParts; i++)
		{
			pointsAux[0] = { 0 + partDistance * i,commonHeight };

			for (int j = 1; j < maxPoints; j++)
			{
				pointsAux[j].y = startY;
				pointsAux[j].x = partDistance * i + distBetweenPoints * j;
				pointsAux[j].y = randomizedY(pointsAux[j]);
			}

			ground.parts[static_cast<int>(static_cast<PartName>(i))].shape = irregular::init(pointsAux, maxPoints);
		}

		ground.parts[static_cast<int>(PartName::Left)].canMove = true;
		ground.parts[static_cast<int>(PartName::Middle)].canMove = false;
		ground.parts[static_cast<int>(PartName::Right)].canMove = false;
	}

	static float randomizedY(vec::Vector2 point)
	{
		for (int i = 0; i < randomRepeatAmount; i++)
		{
			point.y += sin(factor1 * -randomScale1* point.x) + sin(PI * -randomScale2 * point.x) * (randomVariationY * i);
		}

		return point.y;
	}
}