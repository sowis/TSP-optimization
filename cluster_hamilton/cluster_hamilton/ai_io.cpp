#include <algorithm>

#include "ai_io.h"

vector<pair<double, double>> load(void) {
	ifstream tsp_csv(INPUT_FILE);
	if (!tsp_csv) {
		cout << INPUT_FILE << " 파일을 찾을 수 없습니다\n";
		exit(-1);
	}

	vector<pair<double, double>> positions(1000);
	for (pair<double, double>& p : positions) {
		char tmp;
		tsp_csv >> p.first >> tmp >> p.second;
	}

	tsp_csv.close();

	return positions;
}

void save(const vector<int>& result, const string& filename) {
	ofstream save(filename);
	if (!save) {
		cout << filename << " 파일을 여는 데 실패했습니다\n";
		exit(-1);
	}

	for (const int index : result) {
		save << index << "\n";
	}

	save.close();
}


void save_clusters(vector<vector<int>> clusters, const string& filename) {
	int target_cluster = 0;
	int target_node = 0;
	bool find_target = false;
	for (int i = 0; i < clusters.size(); ++i) {
		if (find_target) {
			break;
		}

		for (int node = 0; node < clusters[i].size(); ++node) {
			if (clusters[i][node] == 0) {
				target_cluster = i;
				target_node = node;
				find_target = false;
				break;
			}
		}
	}

	swap(clusters[target_cluster][0], clusters[target_cluster][target_node]);
	swap(clusters[0], clusters[target_cluster]);

	ofstream save(filename);
	if (!save) {
		cout << filename << " 파일을 여는 데 실패했습니다\n";
		exit(-1);
	}

	for (const vector<int>& cluster : clusters) {
		for (int i = 0; i < cluster.size(); ++i) {
			save << cluster[i];
			if (i != cluster.size() - 1) {
				save << "\t";
			}
		}
		save << "\n";
	}

	save.close();
}