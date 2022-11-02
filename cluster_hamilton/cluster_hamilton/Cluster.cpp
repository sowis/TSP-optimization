#include <limits>
#include <algorithm>
#include <deque>

#include "Cluster.h"
#include "ai_utility.h"
#include "Hamilton.h"

class Edge {
public:
	int node1;
	int node2;
	double distance;

	bool operator<(const Edge& e) const {
		return this->distance > e.distance;
	}
};

Cluster::Cluster(const vector<int>& nodes, const vector<pair<double, double>>& positions)
	:nodes(nodes), positions(positions) {
	this->update_hamilton_edges();
}

vector<int> Cluster::get_nodes(void) const {
	return this->nodes;
}

vector<int> Cluster::ban(const int count) {
	/* 주변 노드들과의 거리 평균 구하기 */
	vector<double> average_neighbor_distance(this->nodes.size(), 0);
	for (int node_index = 0; node_index < this->nodes.size(); ++node_index) {
		for (const int neighbor_index : this->hamilton_edges[node_index]) {
			const double distance = dist(this->nodes[node_index], this->nodes[neighbor_index], this->positions);
			average_neighbor_distance[node_index] += distance;
		}

		average_neighbor_distance[node_index] /= this->hamilton_edges[node_index].size();
	}
	/************************************/

	/* 밴할 노드 구하기 */
	vector<bool> ban_check(this->nodes.size(), false);
	for (int i = 0; i < count; ++i) {
		int target = -1;
		double max_dist = numeric_limits<double>::min();

		for (int node_index = 0; node_index < this->nodes.size(); ++node_index) {
			if (ban_check[node_index]) {
				continue;
			}

			if (max_dist < average_neighbor_distance[node_index]) {
				max_dist = average_neighbor_distance[node_index];
				target = node_index;
			}
		}

		ban_check[target] = true;
	}
	/*****************/

	vector<int> banned;

	/* 노드 업데이트 */
	vector<int> new_nodes;
	for (int i = 0; i < this->nodes.size(); ++i) {
		if (ban_check[i] == false) {
			new_nodes.push_back(this->nodes[i]);
		}
		else {
			banned.push_back(this->nodes[i]);
		}
	}
	this->nodes = new_nodes;
	/****************/

	return banned;
}

vector<int> Cluster::random_pop(const int count) {
	vector<bool> poped(this->nodes.size(), false);
	for (int i = 0; i < count; ++i) {
		int target = random(0, (int)this->nodes.size());
		while (poped[target]) {
			target = random(0, (int)this->nodes.size());
		}

		poped[target] = true;
	}

	vector<int> new_nodes;
	vector<int> banned;

	for (int i = 0; i < this->nodes.size(); ++i) {
		if (poped[i]) {
			banned.push_back(this->nodes[i]);
		}
		else {
			new_nodes.push_back(this->nodes[i]);
		}
	}

	this->nodes = new_nodes;
	return banned;
}

void Cluster::add_nodes(const vector<int>& add_nodes) {
	for (const int new_node : add_nodes) {
		this->nodes.push_back(new_node);
	}
}

double Cluster::get_min_dist(const int idx) const {
	double result = numeric_limits<double>::max();
	for (const int node : this->nodes) {
		result = min(result, dist(node, idx, this->positions));
	}

	return result;
}

double Cluster::get_hamilton_sum(void) const {
	return this->hamilton_sum;
}

void Cluster::update_hamilton_edges(void) {
	Hamilton hamilton(this->nodes, this->positions);
	this->hamilton_sum = hamilton.get_cycle_length();
	const deque<int> path = hamilton.get_path();

	this->hamilton_edges = {};
	for (int i = 0; i < path.size() - 1; ++i) {
		this->hamilton_edges[path[i]].insert(path[i + 1]);
		this->hamilton_edges[path[i + 1]].insert(path[i]);
	}
}


