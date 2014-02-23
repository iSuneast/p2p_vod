#pragma once

struct PeerStat {
	PeerStat() {
		clear();
	}

	void clear() {
		enter_time = -1;
		finish_time = -1;
		exit_time = -1;

		start_position = -1;

		latency = 0;
		interrupts = 0;
	}

	void PrintLog(int peer_id) {
		static int log_id = 0;
		log_id++;
		if (log_id == 1) {
			Log::f("Peer Id\tEnter Time\tFinish Time\tStart Position\tLatency\tInterrupts\tSmoothness\tExit Time\n");
		}

		smoothness = -1;
		if (finish_time == -1) {
			if (exit_time - enter_time - latency > 0) {
				smoothness = 1. - interrupts / (double)(exit_time - enter_time - latency);
			}
		} else if (finish_time - enter_time - latency > 0) {
			smoothness = 1. - interrupts / (double)(finish_time - enter_time - latency);
		}
		Log::f("%d\t%d\t%d\t%d\t%d\t%d\t%.5lf\t%d\n",
			peer_id, enter_time, finish_time, start_position, latency, interrupts, smoothness, exit_time);
	}

	int enter_time;
	int finish_time;
	int exit_time;
	int start_position;

	int latency;
	int interrupts;
	double smoothness;
};
