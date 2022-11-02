#pragma once

#include <vector>

#include "Cluster.h"

using namespace std;

class Board {
private:
	const vector<pair<double, double>>& positions;
	const int cluster_size;
	vector<Cluster> clusters;

public:
	Board(const int cluster_size, const vector<pair<double, double>>& positions);

	void next(const int ban_count, const bool mutation, const int mutation_count);
	vector<vector<int>> get_clusters(void) const;
	double get_mst_sum(void) const;
};