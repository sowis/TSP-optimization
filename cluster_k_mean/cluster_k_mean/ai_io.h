#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <iostream>

using namespace std;

const string INPUT_FILE = "TSP.csv";

vector<pair<double, double>> load(void);
void save(const vector<int>& result, const string& filename);
void save_clusters(vector<vector<int>> clusters, const string& filename);