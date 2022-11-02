#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <iostream>

using namespace std;

const string INPUT_FILE = "TSP.csv";
const string OUTPUT_FILE = "all_GA.csv";

vector<pair<double, double>> read_input_file(void);
void save(const vector<int>& result);