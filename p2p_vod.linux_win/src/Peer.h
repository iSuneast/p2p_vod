#pragma once

#include "base.h"
#include "PeerStat.h"
#include "Request.h"

class Peer {
public:
	void Seek(const int &position) {
		peer_stat_.start_position = status_.currently_position = position;
	}

	int get_currently_position() {
		return status_.currently_position;
	}

	void TransmitChunkInit(const int &round_id, bool val) {
		status_.status_bitset.resize(Opt::num_chunks(), val);
		status_.requests_.clear();
		peer_stat_.clear();
		peer_stat_.enter_time = round_id;
		status_.currently_position = 0;
		status_.target_time_stamp = -1;
		status_.buffer = 0;
	}

	int get_buffer() {
		return status_.buffer;
	}

	bool IsNewNeighbor(Peer *neighbor) {
		if (out_neighbors_.find(neighbor)) {
			return false;
		}
		if (in_neighbors_.find(neighbor)) {
			return false;
		}
		if (neighbor == this) {
			return false;
		}
		return true;
	}

	void TransmitChunkRoundInit(const int &round_id) {
		status_.queued_peers.clear();
		status_.queued_chunks.clear();
		status_.new_requests.clear();

		if (!FinishedDownloading()) {
			if (status_.target_time_stamp == -1) {
				status_.target_time_stamp = round_id + status_.buffer;
			}
			if (status_.selfish) {	// SELFISH
				for (int i = 0; i < Opt::max_requests_per_round(); ++i) {
					int position = status_.currently_position + i;
					if (!FinishedDownloading(position)) {
						if (status_.status_bitset.test(position)) {
							continue;
						}
						if (position == peer_stat_.start_position) {
							status_.requests_.MergeRequest(Request(
								status_.currently_position,
								status_.target_time_stamp + i*Opt::num_rounds_per_chunk(),
								Opt::urgency_high()));
						} else {
							if (status_.buffer == 0) {
								status_.requests_.MergeRequest(Request(
									status_.currently_position,
									status_.target_time_stamp + i*Opt::num_rounds_per_chunk(),
									Opt::urgency_high()));
							} else {
								status_.requests_.MergeRequest(Request(
									status_.currently_position,
									status_.target_time_stamp + i*Opt::num_rounds_per_chunk()));
							}
						}
					}
				}
			} else {
				if (status_.currently_position == peer_stat_.start_position) {
					status_.requests_.MergeRequest(Request(
						status_.currently_position, status_.target_time_stamp, Opt::urgency_high()));
				} else {
					if (status_.buffer == 0) {
						status_.requests_.MergeRequest(Request(
							status_.currently_position, status_.target_time_stamp, Opt::urgency_high()));
					} else {
						status_.requests_.MergeRequest(Request(
							status_.currently_position, status_.target_time_stamp));
					}
				}
			}
		}
		status_.requests_.SortRequests();
	}

	void TransmitChunkTryConnect() {
		unordered_set<Peer*> rem;
		int num_out_connections = min(Opt::num_out_connections(), (int)out_neighbors_.size());
		for (int i = 0; i < num_out_connections; ++i) {
			Peer* selected_peer = nullptr;
			do {
				selected_peer = (Peer*)out_neighbors_.GetRandom();
			} while (rem.find(selected_peer) != rem.end());

			rem.insert(selected_peer);
			selected_peer->AddQueuedPeer(this);
		}
	}

	void TransmitChunkResponsePeers(int max_responce, int round_id) {
		int sz = status_.queued_peers.size();
		for (int i = 0; i < sz && i < max_responce; ++i) {
			int id = RandomVariable::Get(i, sz);
			swap(status_.queued_peers[i], status_.queued_peers[id]);

			ParseRequest(status_.queued_peers[i], round_id);
			status_.queued_peers[i]->ParseRequest(this, round_id);
		}
	}

	int TransmitChunkRoundEnded(int round_id) {
		for (int chunk_id_ : status_.queued_chunks) {
			if (chunk_id_ == -1) {
				continue;
			}
			status_.status_bitset.Set(chunk_id_);
		}
		status_.queued_chunks.clear();

		int new_position = status_.status_bitset.FindFirstZero(status_.currently_position);
		if (!FinishedDownloading()) {
			if (status_.buffer <= 0) {
				if (status_.currently_position == peer_stat_.start_position) {
					peer_stat_.latency += Opt::penalty();
				} else {
					peer_stat_.interrupts += Opt::penalty();
				}
				status_.buffer = 0;
			} else {
				--status_.buffer;
			}
			if (new_position > status_.currently_position) {
				status_.buffer += (new_position - status_.currently_position) * Opt::num_rounds_per_chunk();
			}
		} else {
			if (peer_stat_.finish_time == -1) {
				--status_.buffer;
				if (status_.buffer <= 0) {
					peer_stat_.finish_time = round_id;
				}
			}
		}

		if (status_.currently_position != new_position) {
			status_.currently_position = new_position;
			status_.target_time_stamp = -1;
		}
		status_.requests_.MergeRequests(status_.new_requests, status_.status_bitset);
		status_.requests_.CleanRequests(status_.status_bitset);

		return status_.currently_position;
	}

	void set_selfish(bool selfish) {
		status_.selfish = selfish;
	}

	bool FinishedDownloading(int position = -1) {
		if (position == -1) {
			position = status_.currently_position;
		}
		return position == status_.status_bitset.size();
	}

	bool FinishedWatching() {
		return FinishedDownloading() && (status_.buffer == 0);
	}

	void PrintStatus() {
		printf("peer id: %d \t position: %d \t buffer: %d\n", id(), status_.currently_position, status_.buffer);
	}

	void AddNeighbor(Peer* peer, bool is_out_neighbor) {
		bool ok = false;
		if (is_out_neighbor) {
			ok = out_neighbors_.insert(peer);
		} else {
			ok = in_neighbors_.insert(peer);
		}
		if (!ok) {
			Log::err("AddNeighbor failed(out? %c): %d - %d\n",
				is_out_neighbor ? 'y' : 'n', peer->id(), id());
		}
	}

	void RemoveNeighbor(Peer* peer, bool is_out_neighbor) {
		bool ok = false;
		if (is_out_neighbor) {
			ok = out_neighbors_.erase(peer);
		} else {
			ok = in_neighbors_.erase(peer);
		}
		if (!ok) {
			Log::err("RemoveNeighbor failed(out? %c): %d - %d\n",
				is_out_neighbor ? 'y' : 'n', peer->id(), id());
		}
	}

	void clear() {
		id_ = -1;
		status_.clear();
		in_neighbors_.clear();
		out_neighbors_.clear();
		peer_stat_.clear();
	}

	PeerStat& get_peer_stat() {
		return peer_stat_;
	}

	inline const UnorderedVector<Peer*>& get_out_neighbors() const {
		return out_neighbors_;
	}

	inline const UnorderedVector<Peer*>& get_in_neighbors() const {
		return in_neighbors_;
	}

	inline const int get_neighbors_size() const {
		return out_neighbors_.size() + in_neighbors_.size();
	}

	int id() {
		return id_;
	}

	void set_id(int id) {
		id_ = id;
	}

	void ExitNetwork(const int &round_id) {
		peer_stat_.exit_time = round_id;
		peer_stat_.PrintLog(id_);
	}
	
private:
	bool ParseRequest(Peer *peer, int round_id) {
		if (!status_.selfish) {
			status_.new_requests.MergeRequests(
				peer->status_.requests_,
				status_.status_bitset);
		}

		for (auto &request : peer->status_.requests_.requests_) {
			if (status_.status_bitset.test(request.chunk_id_)) {
				peer->AddNewChunks(request.chunk_id_);
				return true;
			}
		}

		return false;
	}

	void AddNewChunks(const int &chunk_id_) {
		status_.queued_chunks.push_back(chunk_id_);
	}

	void AddQueuedPeer(Peer* queuedPeer) {
		status_.queued_peers.push_back(queuedPeer);
	}

	struct PeerStatus {
		int buffer;
		int currently_position;
		int target_time_stamp;
		bool selfish;
		DynamicBitsetEx status_bitset;
		RequestList requests_;
		RequestList new_requests;
		vector<Peer*> queued_peers;
		vector<int> queued_chunks;

		void clear() {
			buffer = 0;
			currently_position = 0;
			target_time_stamp = -1;
			selfish = false;

			status_bitset.reset();
			requests_.clear();
			new_requests.clear();
			queued_peers.clear();
			queued_chunks.clear();
		}
	} status_;

	int id_;
	UnorderedVector<Peer*> out_neighbors_;
	UnorderedVector<Peer*> in_neighbors_;
	PeerStat peer_stat_;
};
