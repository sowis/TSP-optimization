#pragma once

#include <vector>
#include <algorithm>
#include <unordered_set>
#include <limits>
#include <list>

#include "ai_utility.h"

using namespace std;

class GA {
private:
	const vector<pair<double, double>>& positions;
	list<int> indexes;
	unordered_set<int> banned;

public:
	GA(const vector<pair<double, double>>& positions, const vector<int>& indexes);

	vector<int> get_indexes(void) const;
	double get_distance_sum(void) const;
	void next(const int ban_count, const bool mutation, const int mutation_count);

private:
	void ban(const int count);
	void random_ban(const int count);
	void insert(void);
};

