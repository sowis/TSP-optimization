#pragma once
#include <random>
#include <cmath>

#include "Cluster.h"

using namespace std;

int random(const int from, const int to);

vector<int> create_random_indexes(const int sz);

double dist(const int index_1, const int index_2, const vector<pair<double, double>>& positions);

bool validate(const vector<int>& indexes);

bool cluster_validate(const vector<vector<int>>& clusters);

typedef struct CLUSTER_DISTANCE_DATA {
	int node_1;
	int node_2;
	double distance;
} Cluster_distance_data;

Cluster_distance_data cluster_distance(const Cluster& cluster_1, const Cluster& cluster_2, const vector<pair<double, double>>& positions);

Cluster_distance_data cluster_distance(const Cluster& cluster_1, const vector<int>& banned, const Cluster& cluster_2, const vector<pair<double, double>>& positions);

Cluster_distance_data cluster_distance(const Cluster& cluster, const int pos, const vector<pair<double, double>>& positions);

Cluster_distance_data cluster_distance(const Cluster& cluster, const vector<int>& banned, const int pos, const vector<pair<double, double>>& positions);