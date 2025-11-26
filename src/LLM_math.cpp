#include "LLM_math.h"

namespace mth {

	static bool randomSeedSet = false;

	void Clamp(float& num, float min, float max)
	{
		if (num < min) {
			num = min;
		}
		if (num > max) {
			num = max;
		}
	}

	float Clamped(float num, float min, float max)
	{
		float n = num;
		if (num < min) {
			n = min;
		}
		if (num > max) {
			n = max;
		}
		return n;
	}

	float Abs(float& num)
	{
		if (num < 0) {
			num *= -1.0f;
		}
		return num;
	}

	float AsAbs(float num)
	{
		if (num < 0) {
			num *= -1.0f;
		}
		return num;
	}

	float Min(float num1, float num2)
	{
		if (num1 > num2) {
			return num2;
		}
		else {
			return num1;
		}
	}

	float Max(float num1, float num2)
	{
		if (num1 < num2) {
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
		if (max == min) {
			return 0.0f;
		}
		num = (num - min) / (max - min);
		return num;
	}

	float Normalized(float num, float min, float max)
	{
		float result = num;
		if (max == min) {
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
		if (range == 0) {
			return min;
		}

		if (result < 0) {
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
		if (seed == 0) {
			srand(static_cast<unsigned int>(time(nullptr)));
		}
		else {
			srand(static_cast<unsigned int>(time(&seed)));
		}
		randomSeedSet = true;
	}

	int GetRandom(int min, int max)
	{
		if (!randomSeedSet) {
			SetRandomSeed();
		}
		return (int)(rand() % (max - min + 1) + min);
	}

	float GetRandomf(float min, float max)
	{
		if (!randomSeedSet) {
			SetRandomSeed();
		}
		return min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (max - min)));
	}
}