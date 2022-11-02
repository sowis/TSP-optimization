#include "ai_io.h"

vector<pair<double, double>> read_input_file(void) {
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

void save(const vector<int>& result) {
	ofstream save(OUTPUT_FILE);
	if (!save) {
		cout << INPUT_FILE << " 파일을 여는 데 실패했습니다\n";
		exit(-1);
	}

	for (const int index : result) {
		save << index << "\n";
	}

	save.close();
}