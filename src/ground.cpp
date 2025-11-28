#include "ground.h"

#include "LLM_math.h"

namespace ground
{
	static void drawPart(Part part, sf::RenderWindow& window);
	static void movePart(Part& part);
	static void initParts(Ground& ground);
	static void setLimitLine(Part& part);
	static float randomizedY(vec::Vector2 point, float smoothness);
}

namespace ground
{
	static const float distBetweenPoints = 15.f;
	static const int maxPoints = 30;
	static const float commonHeight = 500;

	static const float distanceBetweenParts = distBetweenPoints * (maxPoints - 1) * 12.5f;
	static const int partConnectionDotMargin = 6;

	static const float startY = 500.f;

	static const float factorE = -0.7f;
	static const float factorPi = -0.3f;
	static const float factor1 = -1.2f;
	static const float scaleE = 0.5f;
	static const float scalePi = 0.3f;
	static const float scale1 = -0.7f;
	static const float factorTotal = -0.7f;
	static const int iter = 245;

	static const float limitLineHeight = 10000.f;

	Ground init()
	{
		Ground newGround;

		initParts(newGround);

		newGround.isLooping = false;

		return newGround;
	}

	void update(Ground& ground, car::Car car)
	{
		if (car.transform.position.x >= ground.parts[static_cast<int>(PartName::Right)].shape.points[maxPoints / 2].x)
		{
			if (ground.parts[static_cast<int>(PartName::Left)].canMove)
			{
				movePart(ground.parts[static_cast<int>(PartName::Left)]);
				ground.parts[static_cast<int>(PartName::Left)].canMove = false;
				ground.parts[static_cast<int>(PartName::Right)].canMove = true;
			}
		}

		if (car.transform.position.x >= ground.parts[static_cast<int>(PartName::Left)].shape.points[maxPoints / 2].x)
		{
			if (ground.parts[static_cast<int>(PartName::Right)].canMove)
			{
				movePart(ground.parts[static_cast<int>(PartName::Right)]);
				ground.parts[static_cast<int>(PartName::Right)].canMove = false;
				ground.parts[static_cast<int>(PartName::Left)].canMove = true;
			}
		}

		/*
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
		*/
	}

	void draw(Ground& ground, sf::RenderWindow& window)
	{
		drawPart(ground.parts[static_cast<int>(PartName::Left)], window);
		drawPart(ground.parts[static_cast<int>(PartName::Right)], window);
		std::cout << distanceBetweenParts;
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

		sf::VertexArray linesTempo = sf::VertexArray(sf::PrimitiveType::Lines, 2);

		linesTempo[0].position.x = part.limitLine[0].x;
		linesTempo[0].position.y = part.limitLine[0].y;

		linesTempo[1].position.x = part.limitLine[1].x;
		linesTempo[1].position.y = part.limitLine[1].y;

		window.draw(linesTempo);
	}

	static void movePart(Part& part)
	{
		for (int i = 0; i < part.shape.pointAmount; i++)
		{
			part.shape.points[i].x += distanceBetweenParts * 2;

			if ((i > maxPoints-1 - partConnectionDotMargin && i < maxPoints-1) || (i > 0 && i <= partConnectionDotMargin-1))
			{
				part.shape.points[i].y = randomizedY(part.shape.points[i], 2);
			}
			else if (i == maxPoints-1 || i == 0)
			{
				part.shape.points[i].y = commonHeight;
			}
			else
			{
				part.shape.points[i].y = randomizedY(part.shape.points[i],1);
			}
		}

		setLimitLine(part);
	}

	static void initParts(Ground& ground)
	{
		vec::Vector2 pointsAux[maxPoints];

		for (int i = 0; i < totalParts; i++)
		{
			for (int j = 0; j < maxPoints; j++)
			{
				pointsAux[j].y = startY;
				pointsAux[j].x = distBetweenPoints * (j + 1) * 12.5f;

				if (i != static_cast<int>(PartName::Left))
				{
					pointsAux[j].x += i * distanceBetweenParts;
				}

				if ((j > maxPoints - 1 - partConnectionDotMargin && j < maxPoints-1) || (j > 0 && j <= partConnectionDotMargin - 1 && i != 0))
				{
					pointsAux[j].y = randomizedY(pointsAux[j],2);
				}
				else if (j == maxPoints - 1 || j == 0)
				{
					pointsAux[j].y = commonHeight;
				}
				else
				{
					pointsAux[j].y = randomizedY(pointsAux[j],1);
				}
			}
			
			ground.parts[static_cast<int>(static_cast<PartName>(i))].shape = irregular::init(pointsAux, maxPoints);
			setLimitLine(ground.parts[static_cast<int>(static_cast<PartName>(i))]);
		}

		ground.parts[static_cast<int>(PartName::Left)].canMove = true;
		ground.parts[static_cast<int>(PartName::Right)].canMove = false;
	}

	static void setLimitLine(Part& part)
	{
		part.limitLine[0] = part.shape.points[0];

		part.limitLine[1].x = part.shape.points[0].x;
		part.limitLine[1].y = part.shape.points[0].y - limitLineHeight;
	}

	static float randomizedY(vec::Vector2 point, float smoothness)
	{
		for (int i = 0; i < iter; i++)
		{
			point.y += factorTotal * (factor1 * sin(scale1 * point.x) - factorE * sin(scaleE * point.x) + factorPi * sin(scalePi * PI * point.x)) / smoothness;
		}

		return point.y;
	}
}