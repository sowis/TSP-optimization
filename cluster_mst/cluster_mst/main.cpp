#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <limits>
#include <algorithm>

#include "ai_io.h"
#include "ai_utility.h"
#include "Board.h"

using namespace std;

const int CLUSTER_SIZE = 10; // 클러스터 크기
const int ITERATE_COUNT = 10000; // 반복 횟수
const int BAN_COUNT = 1; // 한 클러스터에서 한번에 떨어져나가는 노드 숫자
const bool MUTATION = true; // 돌연변이 여부
const int MUTATION_COUNT = 1; // 한 세대에서 돌연변이가 나타나는 개수

const string OUTPUT_FILE = "cluster_mst.tsv";

int main(void) {
	const vector<pair<double, double>> positions = load();

	Board board(CLUSTER_SIZE, positions);

	vector<vector<int>> clusters = board.get_clusters();
	double min_mst_sum = board.get_mst_sum();
	int gen = 0;
	printf("%d mst sum: %.2lf\n", gen, min_mst_sum);

	for (int i = 0; i < ITERATE_COUNT; ++i) {
		board.next(BAN_COUNT, MUTATION, MUTATION_COUNT);
		const double current_mst_sum = board.get_mst_sum();
		if (current_mst_sum < min_mst_sum) {
			min_mst_sum = current_mst_sum;
			clusters = board.get_clusters();
			gen = i + 1;
		}

		printf("%d mst sum: %.2lf\n", i + 1, current_mst_sum);
	}

	save_clusters(clusters, OUTPUT_FILE);

	if (cluster_validate(clusters) == false) {
		cout << "유효하지 않은 클러스터\n";
		return -1;
	}

	printf("\n");
	printf("gen: %d\n", gen);
	printf("min mst sum: %.2lf\n", min_mst_sum);

	return 0;
}