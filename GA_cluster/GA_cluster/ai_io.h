#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <iostream>

using namespace std;

vector<pair<double, double>> load(const string& filename);

vector<vector<int>> load_clusters(const string& filename);

void save(const vector<int>& result, const string& filename);

void save_clusters(vector<vector<int>> clusters, const string& filename);