#pragma once

#include <iostream>
#include <cmath>

static constexpr float PI = 3.14159265358979323846f;

namespace mth
{
	void Clamp(float& num, float min = 0, float max = 1.0f);		// clamp number between min and max
	float Clamped(float num, float min = 0, float max = 1.0f);		// clamp number between min and max
	float Abs(float& num);										// makes the number positive
	float AsAbs(float num);										// makes the number positive

	float Min(float num1, float num2);
	float Max(float num1, float num2);

	float RadianToDegree(float angle);							// radians to degrees
	float DegreeToRadian(float angle);							// degrees to radians

	float Normalize(float& num, float min = 0, float max = 1.0f);
	float Normalized(float num, float min = 0, float max = 1.0f);

	float Lerp(float min, float max, float percentage = 0.5f);

	float WrapToRange(float num, float min = 0, float max = 1.0f);
	float MapToRange(float num, float iMin, float iMax, float oMin, float oMax);

	void SetRandomSeed(time_t seed = 0);													// set random seed
	int GetRandom(int min = 0, int max = 100);												// get random int
	float GetRandomf(float min = 0.0f, float max = 1.0f);		// get random float
}

namespace vec
{
	struct Vector2
	{

	private:

		bool isCartesianBool;
		bool isCircularBool;

	public:

		float x;
		float y;

		Vector2(bool cartesian = true);
		Vector2(float x, float y, bool cartesian = true);

		void normalize();											// normalize the vector
		Vector2 normalized();									// returns the normalized vector without modyfing the original
		float magnitude();											// returns the magnitude
		float magnitude(float magnitude);							// scale the vector and returns the new magnitude
		float determinant(Vector2 v);								// determinant between 2 vectors
		void clamp(float max);										// clamp the magnitude to a max value
		void clamp(float min, float max);							// clamp the magnitude between values
		float angle(Vector2 v = { 1,0 });							// angle between 2 vectors. default is {1,0}
		void rotate(float angle);									// rotate the vector by the given angle in radians
		Vector2 rotated(float angle);							// return the rotated vector by the given angle in radians without modyfing the original
		void rotateDegree(float angle);								// rotate the vector by the given angle in degrees
		Vector2 rotatedDegree(float angle);					// return the rotated vector by the given angle in degrees without modyfing the original
		void randomizeAngle();											// randomize the direction of the vector. keep magnitude
		void randomizeAngle(float min, float max);											// randomize the direction of the vector. keep magnitude
		void randomize(float maxMagnitude);							// randomize the direction of the vector. randomize magnitude between 1 and max
		void randomize(float minMagnitude, float maxMagnitude);		// randomize the direction of the vector. randomize magnitude between min and max

		bool isCartesian();
		bool setAsCartesian();
		bool isCircular();
		bool setAsCircular();

		Vector2 asCircular();									// from cartesian (in radians)
		Vector2 asCircularDegree();							// from cartesian (in degrees)
		Vector2 toCircular();									// transform to circular from cartesian (in radians)
		Vector2 toCircularDegree();							// transform to circular from cartesian (in degrees)

		Vector2 asCartesian();									// from circular (in radians)
		Vector2 asCartesianDegree();							// from circular (in degrees)
		Vector2 toCartesian();									// to cartesian from circular (in radians)
		Vector2 toCartesianDegree();							// to cartesian from circular (in degrees)

		Vector2 operator + (const Vector2 vector)
		{
			return Vector2(x + vector.x, y + vector.y);
		}
		Vector2 operator +=  (const Vector2 vector)
		{
			this->x += vector.x;
			this->y += vector.y;
			return *this;
		}
		Vector2 operator - (const Vector2 vector)
		{
			return Vector2(x - vector.x, y - vector.y);
		}
		Vector2 operator -= (const Vector2 vector)
		{
			this->x -= vector.x;
			this->y -= vector.y;
			return *this;
		}
		float operator * (const Vector2 vector)
		{
			return (float)((x * vector.x) + (y * vector.y));
		}
		Vector2 operator * (const float scalar)
		{
			Vector2 result = *this;
			result.x *= scalar;
			result.y *= scalar;
			return result;
		}
		Vector2 operator * (const int scalar)
		{
			Vector2 result = *this;
			result.x *= scalar;
			result.y *= scalar;
			return result;
		}
		Vector2 operator *= (const float scalar)
		{
			x *= scalar;
			y *= scalar;
			return *this;
		}
		Vector2 operator *= (const int scalar) 
		{
			x *= scalar;
			y *= scalar;
			return *this;
		}
		Vector2 operator = (const Vector2 vector)
		{
			this->x = vector.x;
			this->y = vector.y;
			return *this;
		}
		Vector2 operator = (const float value)
		{
			this->x = value;
			this->y = value;
			return *this;
		}
		Vector2 operator = (const int value)
		{
			this->x = static_cast<float>(value);
			this->y = static_cast<float>(value);
			return *this;
		}
		bool operator == (const Vector2 vector)
		{
			if (x == vector.x && y == vector.y)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		bool operator == (const float value)
		{
			if (x == value && y == value)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		bool operator == (const int value)
		{
			if (x == value && y == value)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		bool operator != (const Vector2 vector)
		{
			if (x == vector.x && y == vector.y)
			{
				return false;
			}
			else
			{
				return true;
			}
		}
		bool operator != (const float value)
		{
			if (x == value && y == value)
			{
				return false;
			}
			else
			{
				return true;
			}
		}
		bool operator != (const int value) {
			if (x == value && y == value)
			{
				return false;
			}
			else
			{
				return true;
			}
		}
	};

	std::ostream& operator <<(std::ostream& os, const Vector2& v);
}