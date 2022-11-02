#include <queue>
#include <limits>

#include "Cluster.h"
#include "ai_utility.h"

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
	this->update_mst_edges();
}

vector<int> Cluster::get_nodes(void) const {
	return this->nodes;
}

vector<int> Cluster::ban(const int count) {
	/* 주변 노드들과의 거리 평균 구하기 */
	vector<double> average_neighbor_distance(this->nodes.size(), 0);
	for (int node_index = 0; node_index < this->nodes.size(); ++node_index) {
		for (const int neighbor_index : this->mst_edges[node_index]) {
			const double distance = dist(this->nodes[node_index], this->nodes[neighbor_index], this->positions);
			average_neighbor_distance[node_index] += distance;
		}

		average_neighbor_distance[node_index] /= this->mst_edges[node_index].size();
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

double Cluster::get_mst_sum(void) const {
	return this->mst_sum;
}

void Cluster::update_mst_edges(void) {
	vector<bool> visit(this->nodes.size(), false);
	priority_queue<Edge> q;
	this->mst_edges = {};
	this->mst_sum = 0;

	visit[0] = true;
	for (int i = 1; i < this->nodes.size(); ++i) {
		q.push({ 0, i, dist(this->nodes[0], this->nodes[i], this->positions) });
	}

	while (q.empty() == false) {
		const Edge edge = q.top();
		q.pop();

		if (visit[edge.node1] && visit[edge.node2]) {
			continue;
		}

		const int new_node = visit[edge.node1] ? edge.node2 : edge.node1;
		for (int i = 0; i < this->nodes.size(); ++i) {
			if (i == new_node) {
				continue;
			}

			if (visit[i]) {
				continue;
			}

			q.push({ new_node, i, dist(this->nodes[new_node], this->nodes[i], this->positions) });
		}

		visit[edge.node1] = true;
		visit[edge.node2] = true;

		this->mst_edges[edge.node1].insert(edge.node2);
		this->mst_edges[edge.node2].insert(edge.node1);

		mst_sum += edge.distance;
	}
}