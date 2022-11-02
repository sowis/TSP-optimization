#include <limits>
#include <algorithm>

#include "Kmeans.h"
#include "ai_utility.h"

Kmeans::Kmeans(const int cluster_count, const vector<pair<double, double>>& positions)
	:cluster_count(cluster_count), positions(positions) {
	this->clusters = vector<vector<int>>(cluster_count);
	this->centroids = vector<pair<double, double>>(cluster_count);

	/* 랜덤하게 중심점 잡기 */
	const vector<int> tmp = create_random_indexes((int)positions.size());
	for (int i = 0; i < cluster_count; ++i) {
		const int node = tmp[i];
		this->centroids[i] = positions[node];
	}
	/************************/

	/* 노드 채우기 */
	this->update_nodes();
	/***************/
}

vector<vector<int>> Kmeans::get_clusters(void) const {
	return this->clusters;
}

double Kmeans::get_distance_to_centroids_sum(void) const {
	double result = 0;
	for (int i = 0; i < this->clusters.size(); ++i) {
		const pair<double, double>& centroid = this->centroids[i];
		const vector<int>& cluster = this->clusters[i];

		for (int node : cluster) {
			result += this->distance(centroid, node);
		}
	}

	return result;
}

int Kmeans::get_min_cluster_size(void) const {
	int result = numeric_limits<int>::max();
	for (const vector<int>& cluster : this->clusters) {
		result = min(result, (int)cluster.size());
	}

	return result;
}

int Kmeans::get_max_cluster_size(void) const {
	int result = numeric_limits<int>::min();
	for (const vector<int>& cluster : this->clusters) {
		result = max(result, (int)cluster.size());
	}

	return result;
}

void Kmeans::next(void) {
	this->update_centroids();
	this->update_nodes();
}

double Kmeans::distance(const pair<double, double>& centroid, const int node) const {
	return sqrt(pow(centroid.first - positions[node].first, 2) + pow(centroid.second - positions[node].second, 2));
}

void Kmeans::update_nodes(void) {
	this->clusters = vector<vector<int>>(this->cluster_count);

	for (int node = 0; node < this->positions.size(); ++node) {
		int near_centroid_index = 0;
		double min_distance = numeric_limits<double>::max();
		for (int centroid_index = 0; centroid_index < this->centroids.size(); ++centroid_index) {
			const double dist = this->distance(this->centroids[centroid_index], node);
			if (dist < min_distance) {
				min_distance = dist;
				near_centroid_index = centroid_index;
			}
		}

		this->clusters[near_centroid_index].push_back(node);
	}
}

void Kmeans::update_centroids(void) {
	for (int i = 0; i < this->clusters.size(); ++i) {
		double y_sum = 0, x_sum = 0;
		for (const int node : this->clusters[i]) {
			y_sum += this->positions[node].first;
			x_sum += this->positions[node].second;
		}

		const double y_avg = y_sum / this->clusters[i].size();
		const double x_avg = x_sum / this->clusters[i].size();
		this->centroids[i] = { y_avg, x_avg };
	}
}