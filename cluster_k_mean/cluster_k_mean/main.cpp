#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <limits>

#include "ai_io.h"
#include "ai_utility.h"
#include "Kmeans.h"

using namespace std;

/*
	k-means�� Ŭ������ ������
	k-means�� TEIMS_TO_RUN �� ����
*/

constexpr int CLUSTER_COUNT = 100; // ������ ����
constexpr int TIMES_TO_RUN = 30; // k-means ����ȭ�� �� �� ��������

const string OUTPUT_FILE = "cluster_kmeans.tsv";

int main(void) {
	const vector<pair<double, double>> positions = load();

	Kmeans kmeans(CLUSTER_COUNT, positions);

	double min_distance_to_centroids_sum = kmeans.get_distance_to_centroids_sum();
	int min_cluster_size = kmeans.get_min_cluster_size();
	int max_cluster_size = kmeans.get_max_cluster_size();
	vector<vector<int>> clusters = kmeans.get_clusters();
	int gen = 0;

	printf("gen %d's  distance sum: %.2lf\n", 0,  min_distance_to_centroids_sum);

	for (int i = 0; i < TIMES_TO_RUN; ++i) {
		kmeans.next();

		double distance_to_centroids_sum = kmeans.get_distance_to_centroids_sum();

		printf("gen %d's distance sum: %.2lf\n", i + 1,  distance_to_centroids_sum);

		if (distance_to_centroids_sum < min_distance_to_centroids_sum) {
			min_distance_to_centroids_sum = distance_to_centroids_sum;
			min_cluster_size = kmeans.get_min_cluster_size();
			max_cluster_size = kmeans.get_max_cluster_size();
			clusters = kmeans.get_clusters();
			gen = i + 1;
		}
	}

	if (cluster_validate(clusters) == false) {
		cout << "��ȿ���� ���� Ŭ������\n";
		return -1;
	}

	printf("\n");
	printf("distance sum: %.2lf\n", min_distance_to_centroids_sum);
	printf("generation: %d\n", gen);
	printf("min cluster size: %d\n", min_cluster_size);
	printf("max cluster size: %d\n", max_cluster_size);

	save_clusters(clusters, OUTPUT_FILE);

	return 0;
}