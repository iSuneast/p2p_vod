#pragma once

#include "base.h"

class RandomSeek {
public:
	static int Get() {
		double pr = RandomVariable::UniformRealDice();
		if (pr > Opt::pr_random_seek()) {
			return 0;
		}
		pr = RandomVariable::UniformRealDice();
		int index = upper_bound(cdf.begin(), cdf.end(), pr) - cdf.begin() - 1;
		return index;
	}

	static void Set(int num_chunks) {
		cdf.clear();
		cdf.resize(num_chunks + 1);

		for (int i = 0; i <= num_chunks; ++i) {
			cdf[i] = 1 - exp(-3.*i / num_chunks);
		}
	}

	static void o() {
		double pre = 0;
		for (auto pdf : cdf) {
			cout << pdf - pre << endl;
			pre = pdf;
		}
	}

private:
	static vector<double> cdf;
};

vector<double> RandomSeek::cdf;
