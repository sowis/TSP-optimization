#include "Board.h"
#include "ai_utility.h"

#include <algorithm>
#include <limits>

using namespace std;

Board::Board(const int cluster_size, const vector<pair<double, double>>& positions)
	:cluster_size(cluster_size), positions(positions) {
	const vector<int> random_indexes = create_random_indexes((int)positions.size());
	const int cluster_count = (int)positions.size() / cluster_size;
	vector<vector<int>> cluster_indexes(cluster_count, vector<int>(cluster_size));
	for (int cluster = 0; cluster < cluster_count; ++cluster) {
		for (int i = 0; i < cluster_size; ++i) {
			cluster_indexes[cluster][i] = random_indexes[cluster * cluster_size + i];
		}
	}

	for (int i = 0; i < cluster_count; ++i) {
		this->clusters.push_back(Cluster(cluster_indexes[i], positions));
	}
}

void Board::next(const int ban_count, const bool mutation, const int mutation_count) {
	/* 돌연변이 처리 */
	if (mutation) {
		for (int i = 0; i < mutation_count; ++i) {
			const int target_cluster = random(0, (int)this->clusters.size());
			const int another_clutser = random(0, (int)this->clusters.size());

			const vector<int> target_1 = this->clusters[target_cluster].random_pop(1);
			const vector<int> target_2 = this->clusters[another_clutser].random_pop(1);

			this->clusters[target_cluster].add_nodes(target_2);
			this->clusters[another_clutser].add_nodes(target_1);
		}
	}
	/***************/

	/* 클러스터에서 먼 노드 떼어내기 */
	vector<int> ban_pool;
	for (Cluster& cluster : clusters) {
		const vector<int> banned = cluster.ban(ban_count);
		for (const int node : banned) {
			ban_pool.push_back(node);
		}
	}
	/********************************/

	/* 노드가 새로 들어갈 클러스터 정하기 */
	vector<vector<int>> add_to(this->clusters.size());
	vector<bool> used(ban_pool.size(), false);
	for (int cluster_index = 0; cluster_index < this->clusters.size(); ++cluster_index) {
		for (int i = 0; i < ban_count; ++i) {
			int target_node = 0;
			double min_dist = numeric_limits<double>::max();

			for (int node = 0; node < ban_pool.size(); ++node) {
				if (used[node]) {
					continue;
				}

				const double current_dist = this->clusters[cluster_index].get_min_dist(ban_pool[node]);
				if (current_dist < min_dist) {
					min_dist = current_dist;
					target_node = node;
				}
			}

			used[target_node] = true;
			add_to[cluster_index].push_back(ban_pool[target_node]);
		}
	}
	/**************************************/

	/* 클러스터에 노드 넣기, mst 업데이트 */
	for (int i = 0; i < this->clusters.size(); ++i) {
		this->clusters[i].add_nodes(add_to[i]);
		this->clusters[i].update_mst_edges();
	}
	/*************************************/
}

vector<vector<int>> Board::get_clusters(void) const {
	vector<vector<int>> result;
	for (const Cluster& cluster : this->clusters) {
		result.push_back(cluster.get_nodes());
	}

	int target_index = 0;
	bool find_target = false;
	for (int i = 0; i < result.size(); ++i) {
		if (find_target) {
			break;
		}

		for (const int node : result[i]) {
			if (node == 0) {
				target_index = i;
				find_target = true;
				break;
			}
		}
	}

	swap(result[0], result[target_index]);
	return result;
}

double Board::get_mst_sum(void) const {
	double result = 0;
	for (const Cluster& cluster : this->clusters) {
		result += cluster.get_mst_sum();
	}

	return result;
}