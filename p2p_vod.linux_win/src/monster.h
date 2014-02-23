#pragma once

#include "base.h"
#include "PeerStat.h"
#include "Peer.h"
#include <mutex>
#include <unordered_map>

class Monster {
private:
	Monster() {
		selected_id = -1;
		evil_ = false;
		selfish_peer_ = nullptr;
	}

	static Monster *inst_;
public:
	static Monster *get_inst() {
		return inst_;
	}

	bool evil() {
		return evil_;
	}

	void set_selfish_peer(Peer *selfish_peer) {
		selfish_peer_ = selfish_peer;
	}

	bool find_selfish_peer() {
		return selfish_peer_ != nullptr;
	}

	void output_selfish_log() {
		if (find_selfish_peer()) {
			selfish_peer_->PrintStatus();
		}
	}

	Peer *get_selfish_peer() {
		return selfish_peer_;
	}

	bool kill(int peer_id, int round_id) {
		if (death_note.find(peer_id) == death_note.end()) {
			fprintf(stderr, "(Monster Angry) ID NOT FOUND: %d (round %d)\n", peer_id, round_id);
			exit(0);
		}
		if (peer_id == selected_id) {
			return false;
		}
		return death_note[peer_id].exit_time == round_id;
	}

	bool select(int peer_id) {
		return selected_id == peer_id;
	}

	void Revenge(PeerStat stat) {
		FILE *fp = fopen(Opt::monster_two_world().c_str(), "a+");
		fprintf(fp, "selfish\t%d\t%d\t%d\t%d\t%d\t%d\t%.5lf\t%d\n",
			selected_id,
			stat.enter_time,
			stat.finish_time,
			stat.start_position,
			stat.latency,
			stat.interrupts,
			stat.smoothness,
			stat.exit_time);
		fprintf(stderr, "selfish\t%d\t%d\t%d\t%d\t%d\t%d\t%.5lf\t%d\n\n",
			selected_id,
			stat.enter_time,
			stat.finish_time,
			stat.start_position,
			stat.latency,
			stat.interrupts,
			stat.smoothness,
			stat.exit_time);
		stat = death_note[selected_id];
		fprintf(fp, "social\t%d\t%d\t%d\t%d\t%d\t%d\t%.5lf\t%d\n",
			selected_id,
			stat.enter_time,
			stat.finish_time,
			stat.start_position,
			stat.latency,
			stat.interrupts,
			stat.smoothness,
			stat.exit_time);
		fprintf(stderr, "social\t%d\t%d\t%d\t%d\t%d\t%d\t%.5lf\t%d\n\n",
			selected_id,
			stat.enter_time,
			stat.finish_time,
			stat.start_position,
			stat.latency,
			stat.interrupts,
			stat.smoothness,
			stat.exit_time);
		fclose(fp);
	}

	void feed(const char *evil_log) {
		evil_ = true;

		ifstream fin(evil_log);
		if (fin.fail()) {
			Log::err("# (Monster Angry) NOT FOUND: %s\n", evil_log);
			Opt::EorrorHandle();
			return;
		} else {
			Log::o("# Monster's LOG: %s\n", evil_log);
		}
		int line = 0;
		PeerStat stat;
		int peer_id;
		int no_of_valid_peer = 0;				
		for (string buf; getline(fin, buf); ++line) {
			if (!isdigit(buf[0])) {
				continue;
			}

			istringstream sin(buf);
			sin >> peer_id
				>> stat.enter_time
				>> stat.finish_time
				>> stat.start_position
				>> stat.latency
				>> stat.interrupts
				>> stat.smoothness
				>> stat.exit_time;
			death_note[peer_id] = stat;
		}

		if (Opt::monster_selfish_id() != 0) {
			selected_id = Opt::monster_selfish_id();
		} else {
			cout << "NO ID"<<endl;
			Opt::EorrorHandle();
		}

		fprintf(stderr, "read %d lines %d IDs %d VALIDs from death note: %s\n",
			line,
			death_note.size(),
			no_of_valid_peer,
			evil_log);
		fprintf(stderr, "selected id: %d. Information list below: \n", selected_id);
		fprintf(stderr, "PeerId\tEnterTime\tFinishTime\tStartPosition\tLatency\tInterrupts\tSmoothness\tExitTime\n");
		stat = death_note[selected_id];
		fprintf(stderr, "%d\t%d\t%d\t%d\t%d\t%d\t%.5lf\t%d\n\n",
			selected_id,
			stat.enter_time,
			stat.finish_time,
			stat.start_position,
			stat.latency,
			stat.interrupts,
			stat.smoothness,
			stat.exit_time);
	}

private:
	std::unordered_map<int, PeerStat> death_note;
	int selected_id;
	bool evil_;
	Peer *selfish_peer_;
};

Monster *Monster::inst_ = new Monster();
