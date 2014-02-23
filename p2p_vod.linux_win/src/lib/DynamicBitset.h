#pragma once

#include "Header.h"

class DynamicBitset {
public:
	typedef unsigned int BlockType;
	static const int kBlockDataSize;
	static const int kBitsPerBlock;
	static const int kBlockIdOffset;
	static const int kBlockIdMask;

	DynamicBitset(const int &size = 0) {
		block_size_ = 0;
		resize(size);
	}

	void resize(int new_size, bool val = false) {
		bit_size_ = new_size;
		int new_block_size = BitSizeToBlockSize(bit_size_);
		if (block_size_ == new_block_size) {
			reset(val);
			return;
		}

		block_size_ = new_block_size;
		block.resize(block_size_);
		reset(val);
	}

	int count() {
		int ret = 0;
		for (int i = 0; i < block_size_; ++i) {
			BlockType tmp = block[i];
			for (int j = 0; j < 4; ++j) {
				ret += count_bits_[CleanHighBits(tmp, 8)];
				tmp >>= 8;
			}
		}
		return ret;
	}

	void Set(const int &pos, bool val = true) {
		if (0 <= pos && pos < bit_size_) {
			int bufferId = GetBlockId(pos);
			int offset = GetBlockOffset(pos);
			if (val) {
				block[bufferId] |= POW2(offset);
			} else {
				block[bufferId] &= ~(POW2(offset));
			}
		}
	}

	void reset(const bool val = false) {
		if (val == false) {
			for (int i = 0; i < block_size_; ++i) {
				block[i] = 0;
			}
		} else {
			for (int i = 0; i < block_size_; ++i) {
				block[i] = ~0;
			}
			block[block_size_ - 1] = CleanHighBits(block[block_size_ - 1], GetBlockOffset(bit_size_));
		}
	}

	bool test(const int &pos) const {
		if (0 <= pos && pos < bit_size_) {
			int block_id = GetBlockId(pos);
			int offset = GetBlockOffset(pos);
			return (block[block_id] & POW2(offset)) != 0;
		} else {
			return false;
		}
	}

	int size() const {
		return bit_size_;
	}

	DynamicBitset operator&(const DynamicBitset& t) const {
		DynamicBitset db(max(bit_size_, t.bit_size_));
		int end = BitSizeToBlockSize(min(bit_size_, t.bit_size_));
		for (int i = 0; i < db.block_size_; ++i) {
			if (i < end) {
				db.block[i] = block[i] & t.block[i];
			} else {
				db.block[i] = 0;
			}
		}
		return db;
	}

	DynamicBitset operator|(const DynamicBitset& t) const {
		DynamicBitset db(max(bit_size_, t.bit_size_));
		int sizeA = BitSizeToBlockSize(bit_size_);
		int sizeT = BitSizeToBlockSize(t.bit_size_);
		for (int i = 0; i < db.block_size_; ++i) {
			db.block[i] = 0;
			if (i < sizeA) {
				db.block[i] |= block[i];
			}
			if (i < sizeT) {
				db.block[i] |= t.block[i];
			}
		}
		return db;
	}


	DynamicBitset operator|=(const DynamicBitset& t) {
		int sizeT = BitSizeToBlockSize(t.bit_size_);
		for (int i = 0; i < block_size_; ++i) {
			if (i < sizeT) {
				block[i] |= t.block[i];
			}
		}
		return *this;
	}

	DynamicBitset operator~() const {
		DynamicBitset db(bit_size_);
		for (int i = 0; i < db.block_size_; ++i) {
			db.block[i] = ~block[i];
		}
		if (db.block_size_ != 0) {
			db.block[db.block_size_ - 1] = CleanHighBits(db.block[db.block_size_ - 1], GetBlockOffset(db.bit_size_));
		}
		return db;
	}

	friend ostream& operator << (ostream &ostream, const DynamicBitset &bitset) {
		for (int i = 0; i < bitset.bit_size_; ++i) {
			if (i % 32 == 0 && i != 0) {
				cout << " ";
			}
			cout << bitset.test(i);
		}
		return ostream;
	}

protected:
	BlockType cleanLowBits(const BlockType &var, const int &pos) const {
		return var & (~(POW2(pos) - 1));
	}

	BlockType CleanHighBits(const BlockType &var, const int &pos) const {
		return var & (POW2(pos) - 1);
	}

	int GetBlockId(const int &pos) const {
		return pos >> kBlockIdOffset;
	}

	int GetBlockOffset(const int &pos) const {
		return pos & kBlockIdMask;
	}

	int BitSizeToBlockSize(int bit_size_) const {
		return (bit_size_ + kBitsPerBlock - 1) / kBitsPerBlock;
	}

	int bit_size_;
	int block_size_;
	vector<BlockType> block;

	static const vector<int> count_bits_;
};

const int DynamicBitset::kBlockDataSize = sizeof(DynamicBitset::BlockType);
const int DynamicBitset::kBitsPerBlock = DynamicBitset::kBlockDataSize << 3;
const int DynamicBitset::kBlockIdOffset = []() {
	int ret = 0;
	for (int i = DynamicBitset::kBitsPerBlock; i > 1; i >>= 1) {
		++ret;
	}
	return ret;
} ();
const int DynamicBitset::kBlockIdMask = POW2(DynamicBitset::kBlockIdOffset) - 1;

const vector<int> DynamicBitset::count_bits_ = []() {
	vector<int> ret(256);

	auto GetBitsCount = [](int v) {
		int ret = 0;
		for (; v > 0; v >>= 1) {
			if (v & 1) {
				++ret;
			}
		}
		return ret;
	};

	for (int i = 0; i < 256; ++i) {
		ret[i] = GetBitsCount(i);
	}
	return ret;
} ();
