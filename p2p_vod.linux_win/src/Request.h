#pragma once

#include "base.h"

class Request {
public:
	Request() {
		chunk_id_ = -1;
		ttl_ = -1;
		urgency_ = -1;
		time_stamp_ = -1;
	}

	Request(int chunk_id, int time_stamp, int urgency = Opt::urgency_normal(), int ttl = Opt::hop_limit())
		:chunk_id_(chunk_id), time_stamp_(time_stamp), urgency_(urgency), ttl_(ttl) {
	}

	bool operator<(const Request &t) const {
		if (urgency_ != t.urgency_) {
			return urgency_ > t.urgency_;
		}
		if (time_stamp_ != t.time_stamp_) {
			return time_stamp_ < t.time_stamp_;
		}
		return ttl_ < t.ttl_;
	}

	int chunk_id_;
	int time_stamp_;
	int ttl_;	// time to live
	int urgency_;
};

class RequestList {
public:
	void MergeRequest(const Request new_request) {
		for (auto &request : requests_) {
			if (new_request.chunk_id_ == request.chunk_id_) {
				ChkMax(request.urgency_, new_request.urgency_);
				ChkMax(request.ttl_, new_request.ttl_);
				ChkMin(request.time_stamp_, new_request.time_stamp_);
				return;
			}
		}
		requests_.push_back(new_request);
	}

	void MergeRequests(const RequestList &new_list, const DynamicBitsetEx &bitset) {
		int parsed_requests = 0;
		for (auto new_request : new_list.requests_) {
			parsed_requests++;
			if (parsed_requests > Opt::max_requests_per_round()) {
				break;
			}

			--new_request.ttl_;
			if (bitset.test(new_request.chunk_id_)) {
				continue;
			}
			if (new_request.ttl_ > 0) {
				MergeRequest(new_request);
			}
		}
	}

	void CleanRequests(const DynamicBitsetEx &bitset) {
		for (int i = 0; i < (int)requests_.size(); ) {
			requests_[i].ttl_--;
			if ((requests_[i].ttl_ <= 0) || bitset.test(requests_[i].chunk_id_)) {
				swap(requests_[i], *requests_.rbegin());
				requests_.pop_back();
			} else {
				++i;
			}
		}
	}

	void SortRequests() {
		sort(requests_.begin(), requests_.end());
	}

	void clear() {
		requests_.clear();
	}

	vector<Request> requests_;
};
