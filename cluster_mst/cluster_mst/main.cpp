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

const int CLUSTER_SIZE = 10; // Ŭ������ ũ��
const int ITERATE_COUNT = 10000; // �ݺ� Ƚ��
const int BAN_COUNT = 1; // �� Ŭ�����Ϳ��� �ѹ��� ������������ ��� ����
const bool MUTATION = true; // �������� ����
const int MUTATION_COUNT = 1; // �� ���뿡�� �������̰� ��Ÿ���� ����

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
		cout << "��ȿ���� ���� Ŭ������\n";
		return -1;
	}

	printf("\n");
	printf("gen: %d\n", gen);
	printf("min mst sum: %.2lf\n", min_mst_sum);

	return 0;
}