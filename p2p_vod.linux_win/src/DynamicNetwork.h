#pragma once

#include "base.h"

class DynamicNetwork {
public:
	static bool ExitWhenDownloading() {
		return RandomVariable::UniformRealDice() < Opt::pr_exit_when_downloading();
	}

	static bool ExitAfterFinished() {
		return RandomVariable::UniformRealDice() < Opt::pr_exit_after_finished();
	}

	static bool BeingSelfish() {
		return RandomVariable::UniformRealDice() < Opt::pr_selfish();
	}

	static int GetNumOfNewPeers() {
		return Opt::num_new_peers_per_round();
	}
};
