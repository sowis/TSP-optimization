#include "ai_utility.h"

int random(const int from, const int to) {
	random_device rd;
	mt19937 mt(rd());
	uniform_int_distribution<int> range(from, to - 1);
	return range(mt);
}

/* 처음, 끝이 0번 노드인 랜덤한 인덱스 벡터 반환
   사이즈는 노드갯수 + 1 */
vector<int> create_random_indexes(const int sz) {
	vector<int> result(sz + 1);
	vector<bool> used(sz, false);

	used[0] = true;
	result[0] = 0;
	result[sz] = 0;

	for (int i = 1; i < (int)sz; ++i) {
		int rand_number = random(0, (int)sz);
		while (used[rand_number] == true) {
			rand_number = random(0, (int)sz);
		}

		used[rand_number] = true;
		result[i] = rand_number;
	}

	return result;
}

double dist(const int index_1, const int index_2, const vector<pair<double, double>>& positions) {
	return sqrt(pow(positions[index_1].first - positions[index_2].first, 2) + pow(positions[index_1].second - positions[index_2].second, 2));
}

bool validate(const vector<int>& indexes) {
	vector<int> number_count(1000, 0);
	for (const int index : indexes) {
		++number_count[index];
	}

	if (number_count[0] != 2) {
		return false;
	}

	for (int i = 1; i < 1000; ++i) {
		if (number_count[i] != 1) {
			return false;
		}
	}

	return true;
}

bool cluster_validate(const vector<vector<int>>& clusters) {
	vector<int> used(1000, 0);
	for (const vector<int>& cluster : clusters) {
		for (const int node : cluster) {
			++used[node];
		}
	}

	for (int i = 0; i < 1000; ++i) {
		if (used[i] != 1) {
			return false;
		}
	}

	return true;
}