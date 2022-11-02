#include <limits>

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

Cluster_distance_data cluster_distance(const Cluster& cluster_1, const Cluster& cluster_2, const vector<pair<double, double>>& positions) {
	const vector<int>& nodes_1 = cluster_1.get_nodes();
	const vector<int>& nodes_2 = cluster_2.get_nodes();

	int target_1 = -1, target_2 = -1;
	double min_dist = numeric_limits<double>::max();

	for (int node_1 = 0; node_1 < nodes_1.size(); ++node_1) {
		for (int node_2 = 0; node_2 < nodes_2.size(); ++node_2) {
			const double d = dist(nodes_1[node_1], nodes_2[node_2], positions);
			if (d < min_dist) {
				min_dist = d;
				target_1 = nodes_1[node_1];
				target_2 = nodes_2[node_2];
			}
		}
	}

	return { target_1, target_2, min_dist };
}

Cluster_distance_data cluster_distance(const Cluster& cluster_1, const vector<int>& banned, const Cluster& cluster_2, const vector<pair<double, double>>& positions) {
	const vector<int>& nodes_1 = cluster_1.get_nodes();
	const vector<int>& nodes_2 = cluster_2.get_nodes();

	int target_1 = -1, target_2 = -1;
	double min_dist = numeric_limits<double>::max();

	for (int node_1 = 0; node_1 < nodes_1.size(); ++node_1) {
		bool ban = false;
		for (const int ban_node : banned) {
			if (ban_node == nodes_1[node_1]) {
				ban = true;
				break;
			}
		}

		if (ban) {
			continue;
		}

		for (int node_2 = 0; node_2 < nodes_2.size(); ++node_2) {
			const double d = dist(nodes_1[node_1], nodes_2[node_2], positions);
			if (d < min_dist) {
				min_dist = d;
				target_1 = nodes_1[node_1];
				target_2 = nodes_2[node_2];
			}
		}
	}

	return { target_1, target_2, min_dist };
}

Cluster_distance_data cluster_distance(const Cluster& cluster, const int pos, const vector<pair<double, double>>& positions) {
	const vector<int>& nodes = cluster.get_nodes();

	int target = -1;
	double min_dist = numeric_limits<double>::max();

	for (int node = 0; node < nodes.size(); ++node) {
		const double d = dist(nodes[node], pos, positions);
		if (d < min_dist) {
			min_dist = d;
			target = nodes[node];
		}
	}

	return { target, pos, min_dist };
}

Cluster_distance_data cluster_distance(const Cluster& cluster, const vector<int>& banned, const int pos, const vector<pair<double, double>>& positions) {
	const vector<int>& nodes = cluster.get_nodes();

	int target = -1;
	double min_dist = numeric_limits<double>::max();

	for (int node = 0; node < nodes.size(); ++node) {
		bool ban = false;
		for (const int ban_node : banned) {
			if (ban_node == nodes[node]) {
				ban = true;
				break;
			}
		}

		if (ban) {
			continue;
		}

		const double d = dist(nodes[node], pos, positions);
		if (d < min_dist) {
			min_dist = d;
			target = nodes[node];
		}
	}

	return { target, pos, min_dist };
}