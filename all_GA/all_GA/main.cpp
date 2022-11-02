#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <limits>

#include "ai_io.h"
#include "ai_utility.h"
#include "GA.h"

using namespace std;

constexpr int GENERATION = 100000; // 몇세대까지 돌릴지 선택
constexpr int CHANGE = 10; // 한 세대에 몇 개의 노드가 섞일지 선택
constexpr bool MUTATION = true; // 돌연변이 적용 여부
constexpr int MUTATION_COUNT = 1; // 몇 개의 노드에서 돌연변이가 일어날지 선택

/*
	랜덤으로 초기 상태 생성 후
	'GENERATION' 번 만큼 'CHANGE' 개의 노드가 섞임
	이 때, 양 옆의 노드와 가장 멀리 떨어져있는 노드가 떨어져나간 뒤
	가장 가까운 곳에 붙게 됨

	그중에서 가장 빠른 경로를 찾아서
	그 길이와, 그 경로가 몇 세대에서 등장했는지를 stdout에 출력
	그리고 그 경로의 인덱스 순서를 파일로 출력
*/

/*
	tips
	한 세대에 섞이는 노드가 커지면 빠르게 수렴하지만 진동폭이 커짐
	한 세대에 섞이는 노드가 작아지면 느리게 수렴해서 조금 더 많은 세대 수가 필요함
	돌연변이는 있는게 다양한 시드에서 조금 더 균일한 결과를 얻을 수 있는듯?
	한 세대에서 돌연변이가 일어나는 노드 수가 너무 많아지면 의미있는 결과를 얻기 힘듦
*/

int main(void) {
	const vector<pair<double, double>> positions = read_input_file();

	vector<int> indexes = create_random_indexes((int)positions.size());

	GA board(positions, indexes);

	double min_length = board.get_distance_sum();
	vector<int> result = board.get_indexes();
	int result_gen = 0;
	printf("gen %d -> %.2lf\n", 0, min_length);

	for (int i = 0; i < GENERATION; ++i) {
		board.next(CHANGE, MUTATION, MUTATION_COUNT);
		const double length = board.get_distance_sum();

		printf("gen %d -> %.2lf\n", i + 1, length);
		if (length < min_length) { // 가장 빠른 경로를 찾기 위해..
			min_length = length;
			result = board.get_indexes();
			result_gen = i + 1;
		}
	}

	if (validate(result) == false) {
		cout << "unvalidate\n";
		return 0;
	}

	printf("\nresult\n");
	printf("min length: %.2lf\n", min_length);
	printf("generation: %d\n", result_gen);

	vector<int> r(1000);
	for (int i = 0; i < 1000; ++i) {
		r[i] = result[i];
	}

	save(r);

	return 0;
}