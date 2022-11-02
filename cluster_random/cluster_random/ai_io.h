#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <iostream>

using namespace std;

const string INPUT_FILE = "TSP.csv";
const string OUTPUT_FILE = "cluster_random.tsv";

vector<pair<double, double>> load(void);
void save(const vector<int>& result);
void save_clusters(const vector<vector<int>>& clusters);