#include "ground.h"

#include "LLM_math.h"
#include "color.h"
#include "render.h"

namespace ground
{
	static void drawPart(Part part, sf::RenderWindow& window);
	static void drawGround(Ground ground, sf::RenderWindow& window);
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

	static const float outlineDrawWidth = 5.f;
	static const float limitLineHeight = 10000.f;
	static const float distanceBetweenOutline = outlineDrawWidth + 1.f;
	static const float secondLineDrawWidth = 15.f;

	static const float groundTiles = 8.f;
	static const float groundTilesHeight = 120.f;

	static const color::ColorsName outLineColor = color::ColorsName::DarkGreen;
	static const color::ColorsName secondLineColor = color::ColorsName::LightGreen;

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
	}

	void draw(Ground& ground, sf::RenderWindow& window)
	{
		drawGround(ground, window);
		drawPart(ground.parts[static_cast<int>(PartName::Left)], window);
		drawPart(ground.parts[static_cast<int>(PartName::Right)], window);
	}

	static void drawPart(Part part, sf::RenderWindow& window)
	{
		for (int i = 0; i < part.shape.pointAmount - 1; i++)
		{
			vec::Vector2 outLineStart = part.shape.points[i];
			vec::Vector2 outLineEnd = part.shape.points[i + 1];
			render::drawThickLine(outLineStart, outLineEnd, outlineDrawWidth, window, outLineColor);

			vec::Vector2 line = { part.shape.points[i + 1] - part.shape.points[i] };
			line.normalize();

			vec::Vector2 normal = { -line.y, line.x };

			vec::Vector2 secondLineStart = outLineStart + normal * distanceBetweenOutline;
			vec::Vector2 secondLineEnd = outLineEnd + normal * distanceBetweenOutline;

			render::drawThickLine(secondLineStart, secondLineEnd, secondLineDrawWidth, window, secondLineColor);
		}

		sf::VertexArray linesTempo = sf::VertexArray(sf::PrimitiveType::Lines, 2);

		linesTempo[0].position.x = part.limitLine[0].x;
		linesTempo[0].position.y = part.limitLine[0].y;

		linesTempo[1].position.x = part.limitLine[1].x;
		linesTempo[1].position.y = part.limitLine[1].y;

		window.draw(linesTempo);
	}

	static void drawGround(Ground ground, sf::RenderWindow& window)
	{
		for (int i = 0; i < totalParts; i++)
		{
			for (int j = 0; j < ground.parts[i].shape.pointAmount - 1; j++)
			{
				sf::ConvexShape currGround;

				currGround.setPointCount(3);

				float leftPointX = ground.parts[i].shape.points[j].x;
				float leftPointY = ground.parts[i].shape.points[j].y;

				float rightPointX = ground.parts[i].shape.points[j + 1].x;
				float rightPointY = ground.parts[i].shape.points[j + 1].y;

				currGround.setPoint(0, { leftPointX , leftPointY });
				currGround.setPoint(1, { rightPointX ,rightPointY });

				vec::Vector2 lowest;

				if (leftPointY < rightPointY)
				{
					lowest = { leftPointX ,leftPointY };
				}
				else if (rightPointY <= leftPointY)
				{
					lowest = { rightPointX ,rightPointY };
				}

				float thirdPointX = lowest.x;
				float thirdPointY = fmaxf(leftPointY, rightPointY);

				currGround.setPoint(2, { thirdPointX,thirdPointY });

				sf::Texture* texture = &externs::groundTexture;
				currGround.setTexture(texture);

				window.draw(currGround);

				for (int k = 0; k < groundTiles; k++)
				{
					sf::ConvexShape currTileSheet;

					currTileSheet.setPointCount(4);

					currTileSheet.setPoint(0, { leftPointX,thirdPointY + groundTilesHeight * k });
					currTileSheet.setPoint(1, { rightPointX, thirdPointY + groundTilesHeight * k });
					currTileSheet.setPoint(2, { rightPointX, groundTilesHeight + thirdPointY + groundTilesHeight * k });
					currTileSheet.setPoint(3, { leftPointX, groundTilesHeight + thirdPointY + groundTilesHeight * k });

					currTileSheet.setTexture(texture);

					window.draw(currTileSheet);
				}
			}
		}
	}

	static void movePart(Part& part)
	{
		for (int i = 0; i < part.shape.pointAmount; i++)
		{
			part.shape.points[i].x += distanceBetweenParts * 2;

			if ((i > maxPoints - 1 - partConnectionDotMargin && i < maxPoints - 1) || (i > 0 && i <= partConnectionDotMargin - 1))
			{
				part.shape.points[i].y = randomizedY(part.shape.points[i], 2);
			}
			else if (i == maxPoints - 1 || i == 0)
			{
				part.shape.points[i].y = commonHeight;
			}
			else
			{
				part.shape.points[i].y = randomizedY(part.shape.points[i], 1);
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

				if ((j > maxPoints - 1 - partConnectionDotMargin && j < maxPoints - 1) || (j > 0 && j <= partConnectionDotMargin - 1 && i != 0))
				{
					pointsAux[j].y = randomizedY(pointsAux[j], 2);
				}
				else if (j == maxPoints - 1 || j == 0)
				{
					pointsAux[j].y = commonHeight;
				}
				else
				{
					pointsAux[j].y = randomizedY(pointsAux[j], 1);
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