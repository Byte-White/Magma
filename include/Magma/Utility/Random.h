#ifndef MAGMA_RANDOM_H
#define MAGMA_RANDOM_H
#include <random>

namespace mg
{
	class Random
	{
	public:
		// sets a random seed.
		static void SetSeed()
		{
			s_RandomEngine.seed(std::random_device()());
		}
		static void SetSeed(unsigned int seed)
		{
			s_Seed = seed;
			s_RandomEngine.seed(seed);
		}
		static inline const unsigned int& GetSeed() { return s_Seed; }

		static int Int()
		{
			return s_Distribution(s_RandomEngine);
		}

		static int Int(int min, int max)
		{
			std::uniform_int_distribution<int> distribution(min, max);
			return distribution(s_RandomEngine);
		}

		static uint32_t UInt32()
		{
			return s_Distribution(s_RandomEngine);
		}
		static uint32_t UInt32(uint32_t min, uint32_t max)
		{
			return min + (s_Distribution(s_RandomEngine) % (max - min + 1));
		}

		static int32_t Int32()
		{
			return s_Distribution(s_RandomEngine);
		}
		static int32_t Int32(int32_t min, int32_t max)
		{
			std::uniform_int_distribution<int32_t> distribution(min, max);
			return distribution(s_RandomEngine);
		}

		static uint16_t UInt16()
		{
			return s_Distribution(s_RandomEngine);
		}
		static uint16_t UInt16(uint16_t min, uint16_t max)
		{
			std::uniform_int_distribution<uint16_t> distribution(min, max);
			return distribution(s_RandomEngine);
		}

		static int16_t Int16()
		{
			return s_Distribution(s_RandomEngine);
		}
		static int16_t Int16(int16_t min, int16_t max)
		{
			std::uniform_int_distribution<int16_t> distribution(min, max);
			return distribution(s_RandomEngine);
		}

		static uint8_t UInt8()
		{
			return s_Distribution(s_RandomEngine);
		}
		static uint8_t UInt8(uint8_t min, uint8_t max)
		{
			std::uniform_int_distribution<int> distribution(min, max);
			return static_cast<uint8_t>(distribution(s_RandomEngine));
		}

		static int8_t Int8()
		{
			return s_Distribution(s_RandomEngine);
		}
		static int8_t Int8(int8_t min, int8_t max)
		{
			std::uniform_int_distribution<int> distribution(min, max);
			return static_cast<int8_t>(distribution(s_RandomEngine));
		}

		static float Float()
		{
			return (float)s_Distribution(s_RandomEngine) / (float)std::numeric_limits<uint32_t>::max();
		}

		static float Float(float min, float max)
		{
			std::uniform_real_distribution<float> distribution(min, max);
			return distribution(s_RandomEngine);
		}

		static float Double()
		{
			return (double)s_Distribution(s_RandomEngine) / (double)std::numeric_limits<uint32_t>::max();
		}

		static double Double(double min, double max)
		{
			std::uniform_real_distribution<double> distribution(min, max);
			return distribution(s_RandomEngine);
		}
	private:
		static unsigned int s_Seed;
		static std::mt19937 s_RandomEngine;
		static std::uniform_int_distribution<std::mt19937::result_type> s_Distribution;
	};
}

#endif