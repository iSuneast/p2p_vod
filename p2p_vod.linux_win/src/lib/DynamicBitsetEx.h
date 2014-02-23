#pragma once

#include "DynamicBitset.h"

class DynamicBitsetEx : public DynamicBitset {
public:
	DynamicBitsetEx(const int &size = 0) :
		DynamicBitset(size) {
	}

	int FindKth(int k, const int &pos = 0) const {
		if (k == 0) {
			return FindFirst(pos);
		}
		int ret = 0;
		for (int i = 0; i < block_size_; ++i) {
			BlockType tmp = block[i];
			for (int j = 0, t; j < 4; ++j) {
				t = count_bits_[CleanHighBits(tmp, 8)];
				if (t > k) {
					for (; tmp > 0; tmp >>= 1, ++ret) {
						if (tmp & 1) {
							if (k == 0) {
								return ret;
							}
							--k;
						}
					}
				} else {
					ret += 8;
				}
				k -= t;
				tmp >>= 8;
			}
		}
		return -1;
	}

	int FindFirst(const int &pos = 0) const {
		if (pos < 0 || pos >= bit_size_) {
			return -1;
		}
		int block_id = GetBlockId(pos);
		int offset = GetBlockOffset(pos);
		for (int i = block_id; i < block_size_; ++i) {
			BlockType data = block[i];
			if (i == block_id) {
				data = cleanLowBits(data, offset);
			}
			if (data == 0) {
				continue;
			}
			for (int ret = i*kBitsPerBlock, i = 0; i < 4; ++i, ret += 8) {
				BlockType block = CleanHighBits(data, 8);
				if (first_bit_[block] != -1) {
					return ret + first_bit_[block];
				}
				data >>= 8;
			}
		}
		return -1;
	}

	int FindFirstZero(const int &position) const {
		if (position < 0 || position >= bit_size_) {
			return bit_size_;
		}
		int block_id = GetBlockId(position);
		int offset = GetBlockOffset(position);
		for (int i = block_id; i < block_size_; ++i) {
			BlockType data = ~block[i];
			if (i == block_id) {
				data = cleanLowBits(data, offset);
			}
			if (data == 0) {
				continue;
			}
			for (int ret = i*kBitsPerBlock, i = 0; i < 4; ++i, ret += 8) {
				BlockType block = CleanHighBits(data, 8);
				if (first_bit_[block] != -1) {
					return ret + first_bit_[block];
				}
				data >>= 8;
			}
		}
		return bit_size_;
	}

private:
	static const vector<int> first_bit_;
};

const vector<int> DynamicBitsetEx::first_bit_ = []() {
	vector<int> ret(256);

	auto GetFirstBit = [](int v) {
		int ret = 0;
		for (; v > 0; v >>= 1, ++ret) {
			if (v & 1) {
				return ret;
			}
		}
		return -1;
	};

	for (int i = 0; i < 256; ++i) {
		ret[i] = GetFirstBit(i);
	}
	return ret;
} ();
