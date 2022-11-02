#pragma once

#include <vector>

using namespace std;

class Kmeans {
private:
	const int cluster_count;
	const vector<pair<double, double>> positions;
	vector<vector<int>> clusters;
	vector<pair<double, double>> centroids;

public:
	Kmeans(const int cluster_count, const vector<pair<double, double>>& positions);

	vector<vector<int>> get_clusters(void) const;
	double get_distance_to_centroids_sum(void) const;
	int get_min_cluster_size(void) const;
	int get_max_cluster_size(void) const;
	void next(void);

private:
	double distance(const pair<double, double>& centroid, const int node) const;
	void update_nodes(void);
	void update_centroids(void);
};