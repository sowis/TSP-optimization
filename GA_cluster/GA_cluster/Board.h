#pragma once

#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <list>

#include "Cluster.h"

using namespace std;

class Board {
private:
	const vector<pair<double, double>>& positions;
	list<Cluster> clusters;
	list<pair<int, int>> gates;
	unordered_map<int, unordered_map<int, double>> cluster_distances;
	unordered_map<int, unordered_map<int, pair<int, int>>> cluster_gates;
	vector<Cluster> banned;
	vector<int> path;
	double path_length;

public:
	Board(const vector<vector<int>>& cluster, const vector<pair<double, double>>& positions);

	void next(const int ban_count, const bool mutation, const int mutation_count);
	vector<vector<int>> get_clusters(void) const;
	vector<int> get_path(void) const;
	double get_path_length(void) const;

private:
	void update_path(void);
	void ban(const int count);
	void random_ban(const int count);
	void insert(void);
	double get_cluster_distances(const Cluster& c1, const Cluster& c2);
	pair<int, int> get_cluster_gate(const Cluster& c1, const Cluster& c2);
	int get_second_gate(const Cluster& me, const Cluster& you, const int banned) const;
};