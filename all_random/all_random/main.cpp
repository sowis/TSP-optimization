#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <random>

using namespace std;

const string INPUT_FILE = "TSP.csv";
const string OUTPUT_FILE = "all_random.csv";

vector<pair<double, double>> read_input_file(void);
void save(const vector<int>& result);
int random(const int from, const int to);

int main(void) {
	const vector<pair<double, double>> positions = read_input_file();

	vector<int> result(positions.size());
	vector<bool> used(positions.size(), false);

	used[0] = true;
	result[0] = 0;

	for (int i = 1; i < (int)positions.size(); ++i) {
		int rand_number = random(0, (int)positions.size());
		while (used[rand_number] == true) {
			rand_number = random(0, (int)positions.size());
		}

		used[rand_number] = true;
		result[i] = rand_number;
	}

	save(result);

	return 0;
}

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

int random(const int from, const int to) {
	random_device rd;
	mt19937 mt(rd());
	uniform_int_distribution<int> range(from, to - 1);
	return range(mt);
}
