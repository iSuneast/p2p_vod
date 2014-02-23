#pragma once

#include "Peer.h"

class NetworkBuilder {
public:
	NetworkBuilder(int num_peers = 1) {
		peers_id_ = 0;
		num_new_peers_ = 0;
		for (int i = 0; i < num_peers; ++i) {
			peers_.insert(NewPeer());
		}
	}

	~NetworkBuilder() {
		Log::o("num_new_peers: %d\n", num_new_peers_);
		Log::o("maximum peers_id: %d\n", peers_id_);
		for (auto peer : peers_pool_) {
			delete peer;
		}
		for (auto peer : peers_) {
			delete peer;
		}
	}

	virtual void RemovePeers(vector<Peer*>& list_to_remove) {
		if (list_to_remove.empty()) {
			return;
		}

		vector<Peer*> tmp_in_list;
		vector<Peer*> tmp_out_list;
		for (auto peer : list_to_remove) {
			tmp_in_list.clear();
			tmp_out_list.clear();
			int bns = peer->get_neighbors_size();
			for (auto neighbor : peer->get_out_neighbors()) {
				tmp_out_list.push_back(neighbor);
			}
			for (auto neighbor : peer->get_in_neighbors()) {
				tmp_in_list.push_back(neighbor);
			}

			for (auto neighbor : tmp_out_list) {
				RemoveEdge(peer, neighbor);
			}
			for (auto neighbor : tmp_in_list) {
				RemoveEdge(neighbor, peer);
			}

			// peer should be removed at the beginning
			RemovePeer(peer);

			for (auto neighbor : tmp_in_list) {
				FindNewNeighbor(neighbor, 1);
			}
		}
	}

	virtual vector<Peer*> AddPeers(int num_new_peers) {
		if (num_new_peers == 0) {
			return vector<Peer*>();
		}

		vector<Peer*> list_to_add;
		const vector<Peer*>& peers = GetPeers();
		for (int i = 0; i < num_new_peers; ++i) {
			Peer* peer = NewPeer();
			list_to_add.push_back(peer);
			FindNewNeighbor(peer, min((int)peers.size(), Opt::num_out_neighbors()));
			AddPeer(peer);
		}
		return list_to_add;
	}

	const UnorderedVector<Peer*>& GetPeers() const {
		return peers_;
	}

protected:
	virtual Peer* PickPeer() {
		return GetPeers().GetRandom();
	}

	virtual void RemoveEdge(Peer* from, Peer* to) {
		from->RemoveNeighbor(to, true);
		to->RemoveNeighbor(from, false);
	}

	virtual void AddEdge(Peer *from, Peer *to) {
		from->AddNeighbor(to, true);
		to->AddNeighbor(from, false);
	}

	virtual Peer* NewPeer() {
		Peer *peer = nullptr;
		if (!peers_pool_.empty()) {
			peer = *peers_pool_.rbegin();
			peers_pool_.pop_back();
		} else {
			peer = (Peer*)new Peer();
			num_new_peers_++;
		}
		peer->clear();
		peer->set_id(++peers_id_);

		return peer;
	}

	UnorderedVector<Peer*> peers_;
	
private:
	void FindNewNeighbor(Peer* peer, int num_new_neighbors) {
		Peer *neighbor = nullptr;
		for (int i = 0; i < num_new_neighbors; ++i) {
			while (true) {
				neighbor = PickPeer();
				if (peer->IsNewNeighbor(neighbor)) {
					AddEdge(peer, neighbor);
					break;
				}
			}
		}
	}

	void RemovePeer(Peer *peer) {
		if (peers_.erase(peer)) {
			peers_pool_.push_back(peer);
			peer->clear();
		} else {
			Log::err("NetworkBuilderInfra::RemovePeer, no such peer: %d\n", peer->id());
		}
	}

	void AddPeer(Peer *peer) {
		if (!peers_.insert(peer)) {
			Log::err("NetworkBuilderInfra::AddPeer, peer already in network: %d\n", peer->id());
		}
	}


	vector<Peer*> peers_pool_;
	int peers_id_;
	int num_new_peers_;
};
