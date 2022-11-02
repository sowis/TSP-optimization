#pragma once

#include <vector>
#include <deque>
#include <unordered_map>

using namespace std;

class Cluster {
private:
	const vector<pair<double, double>>& positions;
	vector<int> nodes;
	vector<vector<double>> hamilton_path_length;
	vector<vector<deque<int>>> hamilton_path;

public:
	Cluster(const vector<int>& nodes, const vector<pair<double, double>>& positions);

	double get_path_length(const int begin, const int end) const;
	deque<int> get_path(const int begin, const int end) const;
	vector<int> get_nodes(void) const;

	Cluster& operator=(const Cluster& c);

private:
	void hamilton_path_calculate(const int current_node, const double path_sum, vector<bool>& visit, deque<int>& path);
	int pos_to_index(const int pos) const;
	int index_to_pos(const int index) const;
};