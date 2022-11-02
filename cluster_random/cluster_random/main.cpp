#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <limits>

#include "ai_io.h"
#include "ai_utility.h"

using namespace std;

/*
	랜덤하게 100개의 클러스터를 만듦
	각 클러스터는 10개의 노드를 가지고 있음
*/

int main(void) {
	const vector<pair<double, double>> positions = load();

	vector<int> indexes = create_random_indexes((int)positions.size());

	vector<vector<int>> clusters(100);
	for (int i = 0; i < 100; ++i) {
		for (int k = 0; k < 10; ++k) {
			clusters[i].push_back(indexes[i * 10 + k]);
		}
	}

	save_clusters(clusters);

	return 0;
}