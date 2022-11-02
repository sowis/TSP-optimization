#pragma once

#include <vector>
#include <unordered_map>
#include <unordered_set>

using namespace std;

class Cluster {
private:
	const vector<pair<double, double>>& positions;
	vector<int> nodes;
	unordered_map<int, unordered_set<int>> hamilton_edges;
	double hamilton_sum;

public:
	Cluster(const vector<int>& nodes, const vector<pair<double, double>>& positions);

	vector<int> get_nodes(void) const;
	double get_min_dist(const int idx) const;
	double get_hamilton_sum(void) const;

private:
	vector<int> ban(const int count);
	vector<int> random_pop(const int count);
	void add_nodes(const vector<int>& add_nodes);
	void update_hamilton_edges(void);

	friend class Board;
};

