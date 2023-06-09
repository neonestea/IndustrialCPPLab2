#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <regex>
#include "job.hpp"
#include "multithread_vector.hpp"

using namespace std;

const std::string INPUT_DIR = "../../data/input"; // Путь к входным файлам
const std::string OUTPUT_DIR = "../../data/input"; // Путь для сохранения новых файлов
const std::string OUT_FILES = "out_"; // Префикс имени новых файлов

void splitFiles(const vector<string>& inputFiles, const string& outputDir, const double splitPercent, LockVector <string>& outputFiles) { 
	int totalLines = 0;

	// Подсчет количества строк во всех входных файлах
	for (const string& inputFile : inputFiles) {
		ifstream input(inputFile);
		if (!input.is_open()) {
			cerr << "Error: Unable to open input file1 " << inputFile << endl;
			continue;
		}
		string line;
		while (getline(input, line)) {
            line = regex_replace(line, regex("[[:punct:]]"), "");
			totalLines++;
		}
		input.close();
	}

	// Определение количества строк в каждом новом файле
	const int splitSize = ceil(totalLines * splitPercent);

	// Создание файлов
	int partitionNum = 0;
	int linesWritten = 0;
	string partitionFile = /*outputDir + "/" +*/"partition"s + to_string(partitionNum) + ".txt"s;
	outputFiles.push_back(partitionFile);
	ofstream output(partitionFile);
	if (!output.is_open()) {
		cerr << "Error: Unable to create partition file2 " << partitionFile << endl;
		return;
	}

	// Бежим по всем входным файлам и записываем их строки в partition файлы
	for (const string& inputFile : inputFiles) {
		ifstream input(inputFile);
		if (!input.is_open()) {
			cerr << "Error: Unable to open input file " << inputFile << endl;
			continue;
		}
		string line;
		while (getline(input, line)) {
            // для удаления знаков препинания
            line = regex_replace(line, regex("[[:punct:]]"), "");
			output << line << endl;
			linesWritten++;
			if (linesWritten >= splitSize) {
				// Достигли лимита на количество строк в partition файле
				linesWritten = 0;
				output.close();
				partitionNum++;
				partitionFile = /*outputDir + "/" +*/"partition_" + to_string(partitionNum) + ".txt";
				outputFiles.push_back(partitionFile);
				output.open(partitionFile);
				if (!output.is_open()) {
					cerr << "Error: Unable to create partition file3 " << partitionFile << endl;
					return;
				}
			}
		}
		input.close();
	}
	output.close();
}
