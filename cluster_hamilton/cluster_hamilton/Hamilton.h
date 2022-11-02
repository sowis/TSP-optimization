#pragma once
#include <vector>
#include <deque>

using namespace std;

class Hamilton {
private:
	const vector<pair<double, double>>& positions;
	const vector<int>& nodes;
	double min_cycle_length;
	deque<int> min_path;

public:
	Hamilton(const vector<int>& nodes, const vector<pair<double, double>>& positions);
	double get_cycle_length(void) const;
	deque<int> get_path(void) const;

private:
	void calculate(const int current_node, const double current_length, deque<int>& path, vector<bool>& visit);
};

