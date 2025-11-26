#include "LLM_math.h"

namespace mth
{
	static bool randomSeedSet = false;

	void Clamp(float& num, float min, float max)
	{
		if (num < min)
		{
			num = min;
		}
		if (num > max)
		{
			num = max;
		}
	}

	float Clamped(float num, float min, float max)
	{
		float n = num;
		if (num < min)
		{
			n = min;
		}
		if (num > max)
		{
			n = max;
		}
		return n;
	}

	float Abs(float& num)
	{
		if (num < 0)
		{
			num *= -1.0f;
		}
		return num;
	}

	float AsAbs(float num)
	{
		if (num < 0)
		{
			num *= -1.0f;
		}
		return num;
	}

	float Min(float num1, float num2)
	{
		if (num1 > num2)
		{
			return num2;
		}
		else {
			return num1;
		}
	}

	float Max(float num1, float num2)
	{
		if (num1 < num2)
		{
			return num2;
		}
		else {
			return num1;
		}
	}

	float RadianToDegree(float angle)
	{
		return (angle * 180 / PI);
	}

	float DegreeToRadian(float angle)
	{
		return (angle * PI / 180);
	}

	float Normalize(float& num, float min, float max)
	{
		if (max == min)
		{
			return 0.0f;
		}
		num = (num - min) / (max - min);
		return num;
	}

	float Normalized(float num, float min, float max)
	{
		float result = num;
		if (max == min)
		{
			return 0.0f;
		}
		result = (num - min) / (max - min);
		return num;
	}

	float Lerp(float min, float max, float percentage)
	{
		return min * (1.0f - percentage) + (max * percentage);
	}

	float WrapToRange(float num, float min, float max)
	{
		float range = max - min;
		float result = Clamped(num, min, max);
		if (range == 0)
		{
			return min;
		}

		if (result < 0)
		{
			result += range;
		}
		result = fmodf(num - min, range);
		return result + min;
	}

	float MapToRange(float num, float iMin, float iMax, float oMin, float oMax)
	{
		return iMin + (oMax - oMin) * ((num - iMin) / (iMax - iMin));
	}

	void SetRandomSeed(time_t seed)
	{
		if (seed == 0)
		{
			srand(static_cast<unsigned int>(time(nullptr)));
		}
		else
		{
			srand(static_cast<unsigned int>(time(&seed)));
		}
		randomSeedSet = true;
	}

	int GetRandom(int min, int max)
	{
		if (!randomSeedSet)
		{
			SetRandomSeed();
		}
		return (int)(rand() % (max - min + 1) + min);
	}

	float GetRandomf(float min, float max)
	{
		if (!randomSeedSet)
		{
			SetRandomSeed();
		}
		return min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (max - min)));
	}
}

namespace vec
{
	Vector2::Vector2(bool cartesian)
	{
		isCartesianBool = cartesian;
		isCircularBool = !isCartesianBool;

		x = 0;
		y = 0;
	}

	Vector2::Vector2(float x, float y, bool cartesian)
	{
		isCartesianBool = cartesian;
		isCircularBool = !isCartesianBool;

		this->x = x;
		this->y = y;
	}

	void Vector2::normalize()
	{
		float magnitude = this->magnitude();

		if (magnitude == 0)
		{
			return;
		}

		x /= magnitude;
		y /= magnitude;
	}

	Vector2 Vector2::normalized()
	{
		float magnitude = this->magnitude();

		if (magnitude == 0)
		{
			return { 0,0 };
		}

		Vector2 v = *this;

		v.x /= magnitude;
		v.y /= magnitude;

		return v;
	}

	float Vector2::magnitude()
	{
		return sqrt(x * x + y * y);
	}

	float Vector2::magnitude(float magnitude)
	{
		this->normalize();
		*this *= magnitude;
		return this->magnitude();
	}

	float Vector2::determinant(Vector2 v)
	{
		return ((x * v.y) - (y * v.x));
	}

	void Vector2::clamp(float max)
	{
		if (this->magnitude() > max)
		{
			this->magnitude(max);
		}
	}

	void Vector2::clamp(float min, float max)
	{
		float magnitude = this->magnitude();

		if (magnitude < min)
		{
			this->magnitude(min);
		}
		if (magnitude > max)
		{
			this->magnitude(max);
		}
	}

	float Vector2::angle(Vector2 v)
	{
		Vector2 v1 = *this;
		float angle = 0;

		v1.normalize();
		v.normalize();

		angle = atan2f(v.determinant(v1), v * v1);

		if (angle < 0)
		{
			angle += 2.0f * PI;
		}

		return angle;
	}

	void Vector2::rotate(float angle)
	{
		Vector2 v = *this;

		float cos = cosf(angle);
		float sin = sinf(angle);

		x = v.x * cos - v.y * sin;
		y = v.x * sin + v.y * cos;
	}

	Vector2 Vector2::rotated(float angle)
	{
		Vector2 v = *this;

		float cos = cosf(angle);
		float sin = sinf(angle);

		v.x = x * cos - y * sin;
		v.y = x * sin + y * cos;

		return v;
	}

	void Vector2::rotateDegree(float angle)
	{
		this->rotate(mth::DegreeToRadian(angle));
	}

	Vector2 Vector2::rotatedDegree(float angle)
	{
		return this->rotated(mth::DegreeToRadian(angle));
	}

	void Vector2::randomizeAngle()
	{
		this->rotateDegree(mth::GetRandomf(0.0f, 360.0f));
	}

	void Vector2::randomizeAngle(float min, float max)
	{
		this->rotateDegree(mth::GetRandomf(min, max));
	}

	void Vector2::randomize(float maxMagnitude)
	{
		mth::Clamp(maxMagnitude, 1, maxMagnitude);
		*this = 1;
		this->magnitude(mth::GetRandomf(0, maxMagnitude));
		this->randomizeAngle();
	}

	void Vector2::randomize(float minMagnitude, float maxMagnitude)
	{
		mth::Abs(minMagnitude);
		mth::Clamp(minMagnitude, 1, maxMagnitude);
		mth::Clamp(maxMagnitude, minMagnitude, maxMagnitude);
		*this = 1;
		this->magnitude(mth::GetRandomf(minMagnitude, maxMagnitude));
		this->randomizeAngle();
	}

	bool Vector2::isCartesian()
	{
		return isCartesianBool;
	}

	bool Vector2::setAsCartesian()
	{
		if (isCartesianBool)
		{
			return false;
		}
		isCartesianBool = true;
		isCircularBool = !isCartesianBool;
		return true;
	}

	bool Vector2::isCircular()
	{
		return isCircularBool;
	}

	bool Vector2::setAsCircular()
	{
		if (isCircularBool)
		{
			return false;
		}
		isCircularBool = true;
		isCartesianBool = !isCircularBool;
		return true;
	}

	Vector2 Vector2::asCircular()
	{
		Vector2 v = Vector2(this->magnitude(), this->angle());
		v.setAsCircular();
		return v;
	}

	Vector2 Vector2::asCircularDegree()
	{
		Vector2 v = Vector2(this->magnitude(), mth::RadianToDegree(this->angle()));
		v.setAsCircular();
		return v;
	}

	Vector2 Vector2::toCircular()
	{
		if (isCircularBool)
		{
			return *this;
		}
		isCircularBool = true;
		isCartesianBool = !isCircularBool;
		return *this = Vector2(this->magnitude(), this->angle());
	}

	Vector2 Vector2::toCircularDegree()
	{
		if (isCircularBool)
		{
			return *this;
		}
		isCircularBool = true;
		isCartesianBool = !isCircularBool;
		return *this = Vector2(this->magnitude(), mth::RadianToDegree(this->angle()));
	}

	Vector2 Vector2::asCartesian()
	{
		Vector2 v = Vector2(x * cosf(y), x * sinf(y));
		v.setAsCartesian();
		return v;
	}

	Vector2 Vector2::asCartesianDegree()
	{
		Vector2 v = Vector2(x * cosf(mth::DegreeToRadian(y)), x * sinf(mth::DegreeToRadian(y)));
		v.setAsCartesian();
		return v;
	}

	Vector2 Vector2::toCartesian()
	{
		if (isCartesianBool)
		{
			return *this;
		}
		isCartesianBool = true;
		isCircularBool = !isCartesianBool;
		return *this = Vector2(x * cosf(y), x * sinf(y));
	}

	Vector2 Vector2::toCartesianDegree()
	{
		if (isCartesianBool)
		{
			return *this;
		}
		isCartesianBool = true;
		isCircularBool = !isCartesianBool;
		return *this = Vector2(x * cosf(mth::DegreeToRadian(y)), x * sinf(mth::DegreeToRadian(y)));
	}

	std::ostream& operator <<(std::ostream& os, const Vector2& v)
	{
		return os << v.x << " , " << v.y;
	}
}