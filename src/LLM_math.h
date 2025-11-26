#pragma once

#include <iostream>
#include <cmath>


namespace mth {

	static constexpr float PI = 3.141592;

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