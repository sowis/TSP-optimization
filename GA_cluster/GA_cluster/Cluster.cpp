#include <limits>

#include "Cluster.h"
#include "ai_utility.h"

using namespace std;

Cluster::Cluster(const vector<int>& nodes, const vector<pair<double, double>>& positions)
	:positions(positions) {
	this->nodes = nodes;
	this->hamilton_path_length = vector<vector<double>>(nodes.size(), vector<double>(nodes.size(), numeric_limits<double>::max()));
	this->hamilton_path = vector<vector<deque<int>>>(nodes.size(), vector<deque<int>>(nodes.size()));

	for (int start_node = 0; start_node < this->nodes.size(); ++start_node) {
		vector<bool> visit(this->nodes.size(), false);
		deque<int> path;

		visit[start_node] = true;
		path.push_back(start_node);

		this->hamilton_path_calculate(start_node, 0, visit, path);
	}
}

double Cluster::get_path_length(const int begin, const int end) const {
	return this->hamilton_path_length[this->pos_to_index(begin)][this->pos_to_index(end)];
}

deque<int> Cluster::get_path(const int begin, const int end) const {
	deque<int> result;
	for (int index : this->hamilton_path[this->pos_to_index(begin)][this->pos_to_index(end)]) {
		result.push_back(this->nodes[index]);
	}

	return result;
}

vector<int> Cluster::get_nodes(void) const {
	return this->nodes;
}

Cluster& Cluster::operator=(const Cluster& c) {
	this->nodes = c.nodes;
	this->hamilton_path = c.hamilton_path;
	this->hamilton_path_length = c.hamilton_path_length;
	return *this;
}

void Cluster::hamilton_path_calculate(const int current_node, const double path_sum, vector<bool>& visit, deque<int>& path) {
	if (path.size() == this->nodes.size()) {
		if (path_sum < this->hamilton_path_length[path[0]][current_node]) {
			this->hamilton_path_length[path[0]][current_node] = path_sum;
			this->hamilton_path[path[0]][current_node] = path;
		}
		return;
	}

	for (int next_node = 0; next_node < this->nodes.size(); ++next_node) {
		if (visit[next_node]) {
			continue;
		}

		const double new_length = path_sum + dist(this->nodes[current_node], this->nodes[next_node], this->positions);

		visit[next_node] = true;
		path.push_back(next_node);

		hamilton_path_calculate(next_node, new_length, visit, path);

		path.pop_back();
		visit[next_node] = false;
	}
}

int Cluster::pos_to_index(const int pos) const {
	for (int i = 0; i < this->nodes.size(); ++i) {
		if (this->nodes[i] == pos) {
			return i;
		}
	}

	return -1;
}

int Cluster::index_to_pos(const int index) const {
	return this->nodes[index];
}