#include "Board.h"
#include "ai_utility.h"

#include <algorithm>
#include <iostream>

Board::Board(const vector<vector<int>>& cluster, const vector<pair<double, double>>& positions)
	:positions(positions) {
	for (const vector<int>& c : cluster) {
		this->clusters.push_back(Cluster(c, positions));
	}

	/* 클러스터별 거리 미리 구해놓기 */
	for (const Cluster& cluster_1 : this->clusters) {
		for (const Cluster& cluster_2 : this->clusters) {
			const vector<int> nodes_1 = cluster_1.get_nodes();
			const vector<int> nodes_2 = cluster_2.get_nodes();

			if (nodes_1[0] == nodes_2[0]) {
				continue;
			}

			double min_dist = numeric_limits<double>::max();
			for (const int node_1 : nodes_1) {
				for (const int node_2 : nodes_2) {
					const double current_distance = dist(node_1, node_2, positions);
					if (current_distance < min_dist) {
						min_dist = current_distance;
						this->cluster_gates[nodes_1[0]][nodes_2[0]] = { node_1, node_2 };
					}
				}
			}

			this->cluster_distances[nodes_1[0]][nodes_2[0]] = min_dist;
		}
	}
	/********************************/

	this->update_path();
}

void Board::next(const int ban_count, const bool mutation, const int mutation_count) {
	if (mutation) {
		this->random_ban(mutation_count);
	}

	this->ban(ban_count);
	this->insert();
	this->update_path();
}

vector<vector<int>> Board::get_clusters(void) const {
	vector<vector<int>> result(this->clusters.size());
	for (const Cluster& cluster : this->clusters) {
		result.push_back(cluster.get_nodes());
	}
	return result;
}

vector<int> Board::get_path(void) const {
	return this->path;
}

double Board::get_path_length(void) const {
	return this->path_length;
}

void Board::update_path(void) {
	this->path = {};
	this->path_length = 0;
	this->gates = {};

	int cluster_begin = 0;
	list<Cluster>::iterator it = this->clusters.begin();
	for (; it != --this->clusters.end(); ++it) {
		list<Cluster>::iterator next_it = it;
		++next_it;

		const Cluster_distance_data cluster_data = cluster_distance(*it, { cluster_begin }, *next_it, this->positions);
		const deque<int> cluster_path = (*it).get_path(cluster_begin, cluster_data.node_1);
		for (const int node : cluster_path) {
			this->path.push_back(node);
		}

		this->path_length += (*it).get_path_length(cluster_begin, cluster_data.node_1);
		this->path_length += cluster_data.distance;
		this->gates.push_back({ cluster_begin, cluster_data.node_1 });
		cluster_begin = cluster_data.node_2;
	}

	/* 마지막 클러스터 처리(cycle) */
	const Cluster_distance_data cluster_data = cluster_distance(*it, { cluster_begin }, 0, this->positions);
	const deque<int> cluster_path = (*it).get_path(cluster_begin, cluster_data.node_1);
	for (const int node : cluster_path) {
		this->path.push_back(node);
	}

	this->path_length += (*it).get_path_length(cluster_begin, cluster_data.node_1);
	this->path_length += cluster_data.distance;
	this->gates.push_back({ cluster_begin, cluster_data.node_1 });
	/******************************/
}

void Board::ban(const int count) {
	for (int i = 0; i < count; ++i) {
		/* 밴할 클러스터 정하기 */
		list<Cluster>::iterator target = this->clusters.begin();
		list<pair<int, int>>::iterator target_gate = this->gates.begin();
		list<pair<int, int>>::iterator gate_it = ++this->gates.begin();
		double target_distance = numeric_limits<double>::min();

		for (list<Cluster>::iterator it = ++this->clusters.begin(); it != this->clusters.end(); ++it) {
			list<Cluster>::iterator current = it;
			list<Cluster>::iterator next = it;
			list<Cluster>::iterator last = it;
			++next;
			--last;
			double current_dist = 0;
			if (it == --this->clusters.end()) {
				current_dist = this->get_cluster_distances(*current, *last) + this->get_cluster_distances(*current, *(this->clusters.begin()));
				current_dist += (*it).get_path_length((*gate_it).first, (*gate_it).second);
			}
			else {
				current_dist = this->get_cluster_distances(*current, *last) + this->get_cluster_distances(*current, *next);
				current_dist += (*it).get_path_length((*gate_it).first, (*gate_it).second);
			}

			if (current_dist > target_distance) {
				target_distance = current_dist;
				target = current;
				target_gate = gate_it;
			}

			++gate_it;
		}
		/******************/

		/* 밴 */
		this->banned.push_back(*target);
		this->clusters.erase(target);
		this->gates.erase(target_gate);
		/******/
	}
}

void Board::random_ban(const int count) {
	for (int i = 0; i < count; ++i) {
		int target_count = random(1, (int)this->clusters.size());

		list<Cluster>::iterator target = this->clusters.begin();
		list<pair<int, int>>::iterator target_gate = this->gates.begin();
		for (int k = 0; k < target_count; ++k) {
			++target;
			++target_gate;
		}

		this->banned.push_back(*target);
		this->clusters.erase(target);
		this->gates.erase(target_gate);
	}
}

void Board::insert(void) {
	for (const Cluster& cluster : this->banned) {
		/* 삽입할 곳 정하기 */
		list<Cluster>::iterator target = this->clusters.begin();
		double target_distance = numeric_limits<double>::max();

		int cluster_index = 1;
		for (list<Cluster>::iterator it = ++this->clusters.begin(); ; ++it) {
			list<Cluster>::iterator right = it;
			list<Cluster>::iterator left = it;
			--left;
			double current_distance;

			if (it == this->clusters.end()) {
				current_distance = this->get_cluster_distances(cluster, *left) + this->get_cluster_distances(cluster, *(this->clusters.begin()));
				const int left_gate = this->get_cluster_gate(cluster, *left).first;
				const int right_gate = this->get_second_gate(cluster, *(this->clusters.begin()), left_gate);
				current_distance += cluster.get_path_length(left_gate, right_gate);
			}
			else {
				current_distance = this->get_cluster_distances(cluster, *left) + this->get_cluster_distances(cluster, *right);
				const int left_gate = this->get_cluster_gate(cluster, *left).first;
				const int right_gate = this->get_second_gate(cluster, *right, left_gate);
				current_distance += cluster.get_path_length(left_gate, right_gate);
			}

			if (current_distance < target_distance) {
				target_distance = current_distance;
				target = it;
			}

			if (it == this->clusters.end()) {
				break;
			}

			++cluster_index;
		}
		/*****************/

		/* 삽입 */
		this->clusters.insert(target, cluster);
		/********/
	}

	this->banned = {};
}

double Board::get_cluster_distances(const Cluster& c1, const Cluster& c2) {
	const int node_1 = c1.get_nodes()[0];
	const int node_2 = c2.get_nodes()[0];
	return this->cluster_distances[node_1][node_2];
}

pair<int, int> Board::get_cluster_gate(const Cluster& c1, const Cluster& c2) {
	const int node_1 = c1.get_nodes()[0];
	const int node_2 = c2.get_nodes()[0];
	return this->cluster_gates[node_1][node_2];
}

int Board::get_second_gate(const Cluster& me, const Cluster& you, const int banned) const {
	const vector<int> nodes_1 = me.get_nodes();
	const vector<int> nodes_2 = you.get_nodes();

	int result = nodes_1[0];
	double min_dist = numeric_limits<double>::max();

	for (const int node_1 : nodes_1) {
		if (node_1 == banned) {
			continue;
		}

		for (const int node_2 : nodes_2) {
			const double current_dist = dist(node_1, node_2, this->positions);
			if (current_dist < min_dist) {
				min_dist = current_dist;
				result = node_1;
			}
		}
	}

	return result;
}