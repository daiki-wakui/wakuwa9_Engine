#include "MyRandom.h"
#include <random>

float MyRandom::GetFloatRandom(float min, float max)
{
	float answer;

	std::random_device seed_gen;
	std::mt19937_64 engine(seed_gen());
	std::uniform_real_distribution<float> randValue(min, max);

	answer = randValue(engine);

	return answer;
}
