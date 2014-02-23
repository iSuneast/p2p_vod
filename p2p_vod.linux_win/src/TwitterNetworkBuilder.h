#pragma once

#include "NetworkBuilder.h"

class TwitterNetworkBuilder : public NetworkBuilder {
public:
	TwitterNetworkBuilder()
		:NetworkBuilder(0) {
		printf("Peers Size = %d\n", GetPeers().size());
		FILE *fp = fopen(Opt::twitter_graph_path().c_str(), "r");
		if (fp == nullptr) {
			printf("twitter_graph not found: %s\n", Opt::twitter_graph_path().c_str());
			Opt::EorrorHandle();
		}
		int num_peers;
		fscanf(fp, "%d", &num_peers);
		int num_edges = 0;

		vector<pii> edges;
		for (int u, v; fscanf(fp, "%d %d", &u, &v) != EOF; ) {
			if (id_to_peer.find(u) == id_to_peer.end()) {
				id_to_peer[u] = NewPeer();
			}
			if (id_to_peer.find(v) == id_to_peer.end()) {
				id_to_peer[v] = NewPeer();
			}
			edges.push_back(make_pair(u, v));
			edges.push_back(make_pair(v, u));
		}
		fclose(fp);

		printf("num of peers = %d\n", num_peers);
		printf("num of edges = %d\n", edges.size() >> 1);

		sort(edges.begin(), edges.end());
		
		int real_num_edges = 0;
		for (int i = 0, begin, end; i < (int)edges.size(); i=end) {
			int u = edges[i].first;
			begin = lower_bound(edges.begin(), edges.end(), make_pair(u, -1))
				- edges.begin();
			end = upper_bound(edges.begin(), edges.end(), make_pair(u, INF))
				- edges.begin();

			int num_connections = end - begin;
			Peer* from = id_to_peer[u];
			auto random_val = GetRandomList(num_connections, Opt::num_out_neighbors());
			for (auto v : random_val) {
				Peer *to = id_to_peer[edges[begin + v].second];
				AddEdge(from, to);
				++real_num_edges;
			}
		}
		printf("real num of edges = %d\n", real_num_edges);

		VerifyGraph();
	}

	virtual void RemovePeers(vector<Peer*>& list_to_remove) {
		list_to_remove_.clear();
		for (auto peer : list_to_remove) {
			peer->get_peer_stat().clear();
			list_to_remove_.push_back(peer);
		}
	}

	virtual vector<Peer*> AddPeers(int num_new_peers) {
		// just return the peers to remove
		return list_to_remove_;
	}

protected:
	virtual Peer* NewPeer() {
		Peer *peer = NetworkBuilder::NewPeer();
		peers_.insert(peer);
		fa[peer] = nullptr;
		return peer;
	}

	set<int> GetRandomList(int bound, int num_random) {
		ChkMin(num_random, bound);
		set<int> random_val;
 		while (random_val.size() < num_random) {
 			random_val.insert(RandomVariable::Get(bound));
 		}
		return random_val;
	}
// 	vector<int> GetRandomList(int bound, int num_random) {
// 		ChkMin(num_random, bound);
// 		vector<int> random_val;
// 		for (int i = 0; i < bound; ++i) {
// 			random_val.push_back(i);
// 		}
// 		return random_val;
// 	}

	virtual void AddEdge(Peer *from, Peer *to) {
		NetworkBuilder::AddEdge(from, to);
		Union(from, to);
	}

private:
	void VerifyGraph() {
		set<Peer*> cc;
		for (auto u : fa) {
			cc.insert(find_fa(u.first));
		}
		printf("num of CC: %d\n", cc.size());
		if (cc.size() != 1) {
			Opt::EorrorHandle();
		}
	}

	Peer* find_fa(Peer *u) {
		if (fa[u] == nullptr) {
			return u;
		} else {
			return fa[u] = find_fa(fa[u]);
		}
	}

	void Union(Peer *u, Peer *v) {
		Peer *fa_u = find_fa(u);
		Peer *fa_v = find_fa(v);
		if (fa_u != fa_v) {
			fa[fa_u] = fa_v;
		}
	}

	unordered_map<int, Peer*> id_to_peer;
	unordered_map<Peer*, Peer*> fa;
	vector<Peer*> list_to_remove_;
};
