#pragma once

#include "NetworkBuilder.h"
#include "RandomSeek.h"
#include "DynamicNetwork.h"

class NetworkOperator {
public:
	NetworkOperator(NetworkBuilder *builder)
		:builder(builder), peers((const UnorderedVector<Peer*>&)builder->GetPeers()) {
	}
	
	void Start() {
		TransmitChunkInit();
		int round_id = 1;
		for (; round_id <= Opt::max_running_rounds(); ++round_id) {
			if (!Transmiting(round_id)) {
				break;
			}
		}

		--round_id;
		PrintLog(round_id);
	}

private:
	void PrintLog(int rounds) {
		// Monster RULING !!!!!!!!!!!!!!!!!!!!!!!!!!!
		if (Monster::get_inst()->evil()) {
			Monster *monster = Monster::get_inst();
			for (auto peer : peers) {
				if (monster->select(peer->id()) && peer->FinishedWatching()) {
					peer->ExitNetwork(rounds);
					monster->Revenge(peer->get_peer_stat());
					exit(0);
				}
			}
		}
		// END OF Monster !!!!!!!!!!!!!!!!!!!!!!!!!!!

		for (auto peer : peers) {
			peer->ExitNetwork(rounds);
		}
	}

	bool Transmiting(int round_id) {
		int maxResponse = Opt::max_response();

		for (int i = 0; i < (int)peers.size(); ++i) {
			peers[i]->TransmitChunkRoundInit(round_id);
		}

		for (int i = 0; i < (int)peers.size(); ++i) {
			peers[i]->TransmitChunkTryConnect();
		}

		for (int i = 0; i < (int)peers.size(); ++i) {
			peers[i]->TransmitChunkResponsePeers(maxResponse, round_id);
		}

		int cnt = 0;
		map<int, int> network_status;
		for (int i = 0; i < (int)peers.size(); ++i) {
			int pos = peers[i]->TransmitChunkRoundEnded(round_id);
			if (peers[i]->FinishedDownloading()) {
				++cnt;
			}
			network_status[pos]++;
		}

		int large = 0;
		for (auto it : network_status) {
			ChkMax(large, it.second);
		}

		bool done = true;
		if (UpdateNetwork(round_id)) {
			done = false;
		}

		if (cnt != peers.size()) {
			done = false;
		}
		return !done;
	}

	bool UpdateNetwork(int round_id) {
		vector<Peer*> exit_peers;
		for (auto peer : peers) {
			if (peer == source_peer_) {
				continue;
			}
			if (peer->FinishedWatching()) {
				if (DynamicNetwork::ExitAfterFinished()) {
					exit_peers.push_back(peer);
				}
			} else {
				if (DynamicNetwork::ExitWhenDownloading()) {
					exit_peers.push_back(peer);
				}
			}
		}

		// Monster RULING !!!!!!!!!!!!!!!!!!!!!!!!!!!
		if (Monster::get_inst()->evil()) {
			Monster *monster = Monster::get_inst();

			if (monster->find_selfish_peer()) {
				Peer *selfish_peer = monster->get_selfish_peer();
				if (selfish_peer->FinishedDownloading()) {
					int exit_time = round_id + selfish_peer->get_buffer();
					selfish_peer->get_peer_stat().finish_time = exit_time;
					selfish_peer->ExitNetwork(exit_time);
					monster->Revenge(selfish_peer->get_peer_stat());
					exit(0);
				}
			}

			exit_peers.clear();

			for (auto peer : peers) {
				if (monster->select(peer->id()) && peer->FinishedWatching()) {
					peer->ExitNetwork(round_id);
					monster->Revenge(peer->get_peer_stat());
					exit(0);
				}
				if (monster->kill(peer->id(), round_id)) {
					exit_peers.push_back(peer);
				}
			}
		}
		// END OF Monster !!!!!!!!!!!!!!!!!!!!!!!!!!!

		for (auto exitPeer : exit_peers) {
			exitPeer->ExitNetwork(round_id);
		}

		builder->RemovePeers(exit_peers);

		int num_new_peers = DynamicNetwork::GetNumOfNewPeers();
		if ((int)peers.size() >= Opt::num_peers()) {
			if ((int)(peers.size() - exit_peers.size()) >= Opt::num_peers()) {
				num_new_peers = 0;
			} else {
				num_new_peers = Opt::num_peers() - peers.size() + exit_peers.size();
			}
		} else {
			if ((int)exit_peers.size() > num_new_peers) {
				num_new_peers += exit_peers.size();
			}
		}
		vector<Peer*> new_peers = builder->AddPeers(num_new_peers);

		for (auto peer: new_peers) {
			peer->TransmitChunkInit(round_id, false);
			peer->set_selfish(DynamicNetwork::BeingSelfish());
			peer->Seek(RandomSeek::Get());
		}

		// Monster RULING AGIAN!!!!!!!!!!!!!!!!!!!!!!!!!!!
		if (!Monster::get_inst()->find_selfish_peer()) {
			Monster *monster = Monster::get_inst();
			for (auto peer : new_peers) {
				if (monster->select(peer->id())) {
					peer->set_selfish(true);
					monster->set_selfish_peer((Peer*)peer);
					fprintf(stderr, "selected peer FOUND in round %d.\n", round_id);
					break;
				}
			}
		}
		// END OF Monster !!!!!!!!!!!!!!!!!!!!!!!!!!!
		
		if (round_id % 100 == 0) {
			cout << "round id\t" << round_id << endl;
			if (Monster::get_inst()) {
				Monster::get_inst()->output_selfish_log();
			}
		}

		return new_peers.size() > 0;
	}

	void TransmitChunkInit() {
		for (auto peer : peers) {
			peer->TransmitChunkInit(0, false);
			peer->set_selfish(DynamicNetwork::BeingSelfish());
			peer->Seek(RandomSeek::Get());
		}

		source_peer_ = peers[0];
		source_peer_->TransmitChunkInit(0, true);
		source_peer_->Seek(Opt::num_chunks());
	}

	NetworkBuilder *builder;
	const UnorderedVector<Peer*>& peers;
	Peer* source_peer_;
};
