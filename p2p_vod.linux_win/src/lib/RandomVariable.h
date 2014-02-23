#pragma once

#include <ctime>
#include <random>
#include <functional>

class RandomVariable {
public:
	static int Get(size_t lower_closed_interval, size_t upper_open_interval) {
		// [lower_closed_interval, upper_open_interval)		
		return Get(upper_open_interval - lower_closed_interval) + lower_closed_interval;
	}

	static size_t Get(size_t open_interval) {
		// [0, open_interval)
		return (size_t)(open_interval * UniformRealDice());
	}

	static double UniformRealDice() {
		// [0 , 1)
		static std::uniform_real_distribution<> distribution;

		if (seed) {
			static std::default_random_engine generator_seed_((unsigned)time(nullptr));
			return distribution(generator_seed_);
		} else {
			static std::default_random_engine generator_;
			return distribution(generator_);
		}
	}

	static bool seed;
};

bool RandomVariable::seed = false;
