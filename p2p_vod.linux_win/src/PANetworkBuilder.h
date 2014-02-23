#pragma once

#include "base.h"
#include "NetworkBuilder.h"

class PANetworkBuilder : public NetworkBuilder {
protected:
	virtual Peer* PickPeer() {
		if (edges.size() == 0) {
			Log::err("PANetworkBuilder: edges.size() == 0\n");
			return NetworkBuilder::PickPeer();
		}
		LARGE_INT value = edges.GetRandom();
		return ((PeerKey*)&value)->peer;
	}

	virtual void RemoveEdge(Peer* from, Peer* to) {
		check(
			edges.erase(make(from, from->get_neighbors_size())),
			edges.erase(make(to, to->get_neighbors_size()))
			);

		NetworkBuilder::RemoveEdge(from, to);
	}

	virtual void AddEdge(Peer *from, Peer *to) {
		NetworkBuilder::AddEdge(from, to);

		check(
			edges.insert(make(from, from->get_neighbors_size())),
			edges.insert(make(to, to->get_neighbors_size()))
			);
	}

private:
	typedef unsigned long long LARGE_INT;

	struct PeerKey {
		Peer *peer;
		unsigned int key;
	};

	LARGE_INT make(Peer *peer, int key) {
		PeerKey peer_key;
		peer_key.peer = peer;
		peer_key.key = key;
		LARGE_INT lint = *((LARGE_INT*)&peer_key);
		return lint;
	}

	void check(bool a, bool b) {
		if (!a || !b) {
			printf("# PANetworkBuilder failed\n");
		}
	}

	UnorderedVector<LARGE_INT> edges;
};
