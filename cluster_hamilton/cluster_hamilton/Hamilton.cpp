#include <algorithm>
#include <limits>

#include "Hamilton.h"
#include "ai_utility.h"

using namespace std;

Hamilton::Hamilton(const vector<int>& nodes, const vector<pair<double, double>>& positions)
	:nodes(nodes), positions(positions), min_cycle_length(numeric_limits<double>::max()) {
	vector<bool> visit(this->nodes.size(), false);
	deque<int> path;

	path.push_back(0);
	visit[0] = true;
	calculate(0, 0, path, visit);
}

double Hamilton::get_cycle_length(void) const {
	return this->min_cycle_length;
}

deque<int> Hamilton::get_path(void) const {
	return this->min_path;
}

void Hamilton::calculate(const int current_node, const double current_path_length, deque<int>& path, vector<bool>& visit) {
	if (path.size() == this->nodes.size()) {
		const double cycle_distance = dist(this->nodes[current_node], this->nodes[0], this->positions);
		if (this->min_cycle_length > current_path_length + cycle_distance) {
			this->min_cycle_length = current_path_length + cycle_distance;
			this->min_path = path;
		}
		return;
	}

	for (int next_node_index = 0; next_node_index < this->nodes.size(); ++next_node_index) {
		if (visit[next_node_index]) {
			continue;
		}

		const double distance = current_path_length + dist(this->nodes[current_node], this->nodes[next_node_index], this->positions);
		if (distance >= this->min_cycle_length) {
			continue;
		}

		visit[next_node_index] = true;
		path.push_back(next_node_index);

		calculate(next_node_index, distance, path, visit);

		path.pop_back();
		visit[next_node_index] = false;
	}
}