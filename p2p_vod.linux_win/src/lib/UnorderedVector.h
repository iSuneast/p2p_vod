#pragma once

#include <unordered_map>
#include <vector>
#include "RandomVariable.h"

/*
vector & hash map:
O(1) insert
O(1) erase
O(1) uniformly random selection

WARNING: don't preserve insertion order
*/

template<class Key>
class UnorderedVector :
	public vector<Key> {
	typedef vector<Key> VECTOR;
public:
	void push_back(const Key &key) = delete;
	void pop_back() = delete;

	bool insert(const Key &key) {
		if (key_to_index_.find(key) != key_to_index_.end()) {
			return false;
		}

		key_to_index_[key] = VECTOR::size();
		VECTOR::push_back(key);
		return true;
	}

	bool find(const Key &key) {
		return key_to_index_.find(key) != key_to_index_.end();
	}

	bool erase(const Key &key) {
		if (key_to_index_.find(key) == key_to_index_.end()) {
			return false;
		}

		int index = key_to_index_[key];
		key_to_index_[VECTOR::back()] = index;
		VECTOR::at(index) = VECTOR::back();

		VECTOR::pop_back();
		key_to_index_.erase(key);
		return true;
	}

	Key GetRandom() const {
		return VECTOR::at(RandomVariable::Get(VECTOR::size()));
	}

	void clear() {
		VECTOR::clear();
		key_to_index_.clear();
	}
private:
	std::unordered_map<Key, int> key_to_index_;
};
