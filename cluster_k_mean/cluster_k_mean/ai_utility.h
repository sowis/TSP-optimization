#pragma once
#include <random>
#include <cmath>

using namespace std;

int random(const int from, const int to);

vector<int> create_random_indexes(const int sz);

double dist(const int index_1, const int index_2, const vector<pair<double, double>> &positions);

bool validate(const vector<int>& indexes);

bool cluster_validate(const vector<vector<int>>& clusters);