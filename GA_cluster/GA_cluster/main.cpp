#include <iostream>
#include <string>
#include <vector>

#include "Board.h"
#include "ai_io.h"
#include "ai_utility.h"

using namespace std;

const string POSITION_FILE = "TSP.csv";
const string OUTPUT_FILE = "ga_cluster.csv";

const int GENERATION = 30000;
const int CHANGE_COUNT = 5;
const bool MUTATION = true;
const int MUTATION_COUNT = 1;

int main(int argc, const char* argv[]) {
	if (argc != 2) {
		cout << "����: this.exe  cluster_file_name\n";
		return 0;
	}

	const string CLUSTER_FILE(argv[1]);

	const vector<pair<double, double>> positions = load(POSITION_FILE);
	const vector<vector<int>> clusters = load_clusters(CLUSTER_FILE);

	if (cluster_validate(clusters) == false) {
		cout << "��ȿ���� ���� Ŭ�������Դϴ�\n";
		return -1;
	}

	printf("Ŭ�����ͺ� �ع��� ��� ����ϴ� ��...(5�������ɸ�)\n");
	Board board(clusters, positions);
	vector<int> min_path = board.get_path();
	double min_path_length = board.get_path_length();
	int target_gen = 0;

	printf("gen %d -> %.2lf\n", 0, min_path_length);

	for (int i = 0; i < GENERATION; ++i) {
		board.next(CHANGE_COUNT, MUTATION, MUTATION_COUNT);

		const double current_path_length = board.get_path_length();
		if (min_path_length > current_path_length) {
			min_path_length = current_path_length;
			min_path = board.get_path();
			target_gen = i + 1;
		}

		printf("gen %d -> %.2lf\n", i + 1, current_path_length);
	}

	printf("\n");
	printf("result\n");
	printf("gen %d\n", target_gen);
	printf("path length: %.2lf\n", min_path_length);

	save(min_path, OUTPUT_FILE);

	return 0;
}