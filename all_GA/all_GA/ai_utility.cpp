#include "ai_utility.h"

int random(const int from, const int to) {
	random_device rd;
	mt19937 mt(rd());
	uniform_int_distribution<int> range(from, to - 1);
	return range(mt);
}

/* 처음이 노드인 랜덤한 인덱스 벡터 반환 */
vector<int> create_random_indexes(const int sz) {
	vector<int> result(sz);
	vector<bool> used(sz, false);

	used[0] = true;
	result[0] = 0;

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

	for (int i = 0; i < 1000; ++i) {
		if (number_count[i] != 1) {
			return false;
		}
	}

	return true;
}