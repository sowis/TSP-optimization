#include "GA.h"

#include <limits>

using namespace std;

GA::GA(const vector<pair<double, double>>& positions, const vector<int>& indexes)
:positions(positions) {
	for (const int node : indexes) {
		this->indexes.push_back(node);
	}
}

vector<int> GA::get_indexes(void) const {
	vector<int> result;
	for (const int node : this->indexes) {
		result.push_back(node);
	}

	return result;
}

double GA::get_distance_sum(void) const {
	double result = 0;
	int last = *(this->indexes.begin());
	for (auto it = ++this->indexes.begin(); it != this->indexes.end(); ++it) {
		result += dist(last, *it, this->positions);
		last = *it;
	}

	result += dist(*(this->indexes.begin()), *(--this->indexes.end()), this->positions);

	return result;
}

void GA::next(const int ban_count, const bool mutation, const int mutation_count) {
	if (mutation) {
		this->random_ban(mutation_count);
	}

	this->ban(ban_count);
	this->insert();
}

void GA::ban(const int count) {
	for (int counter = 0; counter < count; ++counter) {
		list<int>::iterator target = this->indexes.begin();
		double target_distance = numeric_limits<double>::max();

		for (list<int>::iterator it = ++this->indexes.begin(); it != this->indexes.end(); ++it) {
			list<int>::iterator left = it;
			list<int>::iterator right = it;

			--left;
			++right;

			if (right == this->indexes.end()) {
				right = this->indexes.begin();
			}

			const double left_dist = dist(*left, *it, this->positions);
			const double right_dist = dist(*it, *right, this->positions);
			const double dist_avg = (left_dist + right_dist) / 2;

			if (dist_avg < target_distance) {
				target_distance = dist_avg;
				target = it;
			}
		}

		this->banned.insert(*target);
		this->indexes.erase(target);
	}
}

void GA::random_ban(const int count) {
	for (int counter = 0; counter < count; ++counter) {
		const int target = random(1, (int)this->indexes.size() - 1);
		list<int>::iterator it = this->indexes.begin();
		for (int i = 0; i < target; ++i) {
			++it;
		}

		this->banned.insert(*it);
		this->indexes.erase(it);
	}
}

void GA::insert(void) {
	for (const int node : this->banned) {
		/* 넣을 자리 찾기 */
		list<int>::iterator target = this->indexes.begin();
		double target_distance = numeric_limits<double>::max();

		for (list<int>::iterator it = ++this->indexes.begin(); it != this->indexes.end(); ++it) {
			list<int>::iterator left = it;
			list<int>::iterator right = it;

			--left;

			const double left_dist = dist(*left, node, this->positions);
			const double right_dist = dist(node, *right, this->positions);
			const double dist_avg = (left_dist + right_dist) / 2;

			if (dist_avg < target_distance) {
				target_distance = dist_avg;
				target = it;
			}
		}
		/*****************/

		/* 마지막 자리 처리 */
		list<int>::iterator left = --this->indexes.end();
		list<int>::iterator right = this->indexes.begin();

		const double left_dist = dist(*left, node, this->positions);
		const double right_dist = dist(node, *right, this->positions);
		const double dist_avg = (left_dist + right_dist) / 2;

		if (dist_avg < target_distance) {
			target = this->indexes.end();
		}
		/*******************/

		this->indexes.insert(target, node);
	}

	this->banned = {};
}