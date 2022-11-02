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

constexpr int GENERATION = 100000; // ������ ������ ����
constexpr int CHANGE = 10; // �� ���뿡 �� ���� ��尡 ������ ����
constexpr bool MUTATION = true; // �������� ���� ����
constexpr int MUTATION_COUNT = 1; // �� ���� ��忡�� �������̰� �Ͼ�� ����

/*
	�������� �ʱ� ���� ���� ��
	'GENERATION' �� ��ŭ 'CHANGE' ���� ��尡 ����
	�� ��, �� ���� ���� ���� �ָ� �������ִ� ��尡 ���������� ��
	���� ����� ���� �ٰ� ��

	���߿��� ���� ���� ��θ� ã�Ƽ�
	�� ���̿�, �� ��ΰ� �� ���뿡�� �����ߴ����� stdout�� ���
	�׸��� �� ����� �ε��� ������ ���Ϸ� ���
*/

/*
	tips
	�� ���뿡 ���̴� ��尡 Ŀ���� ������ ���������� �������� Ŀ��
	�� ���뿡 ���̴� ��尡 �۾����� ������ �����ؼ� ���� �� ���� ���� ���� �ʿ���
	�������̴� �ִ°� �پ��� �õ忡�� ���� �� ������ ����� ���� �� �ִµ�?
	�� ���뿡�� �������̰� �Ͼ�� ��� ���� �ʹ� �������� �ǹ��ִ� ����� ��� ����
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
		if (length < min_length) { // ���� ���� ��θ� ã�� ����..
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