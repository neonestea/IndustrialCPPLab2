#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <filesystem>  

using namespace std;
namespace fs = std::filesystem;  // Создаю псевдоним для пространства имен файловой системы

const std::string INPUT_DIR = "../../projects/src/input/"; // Путь к входным файлам
const std::string OUTPUT_DIR = "../../projects/src/output/"; // Путь для сохранения новых файлов
const std::string OUT_FILE = "out_"; // Префикс имени новых файлов

void splitFiles(const double splitPercent, vector<string>& outputFiles) {
    int totalLines = 0;
    vector<string> inputFiles;

    // Получение списка файлов из INPUT_DIR
    for (const auto& entry : fs::directory_iterator(INPUT_DIR)) {
        if (entry.is_regular_file()) {
            inputFiles.push_back(entry.path().filename().string());
        }
    }

    // Подсчет количества строк во всех входных файлах
    for (const string& inputFile : inputFiles) {
        ifstream input(INPUT_DIR + inputFile);
        if (!input.is_open()) {
            cerr << "Error: Unable to open input file " << inputFile << endl;
            continue;
        }
        string line;
        while (getline(input, line)) {
            totalLines++;
        }
        input.close();
    }

    // Определение количества строк в каждом новом файле
    const int splitSize = ceil(totalLines * splitPercent);

    // Создание файлов
    int partitionNum = 0;
    int linesWritten = 0;
    string partitionFile = OUTPUT_DIR + OUT_FILE + to_string(partitionNum) + ".txt";
    outputFiles.push_back(partitionFile);
    ofstream output(partitionFile);
    if (!output.is_open()) {
        cerr << "Error: Unable to create partition file " << partitionFile << endl;
        return;
    }

    // Бежим по всем входным файлам и записываем их строки в partition файлы
    for (const string& inputFile : inputFiles) {
        ifstream input(INPUT_DIR + inputFile);
        if (!input.is_open()) {
            cerr << "Error: Unable to open input file " << inputFile << endl;
            continue;
        }
        string line;
        while (getline(input, line)) {
            output << line << endl;
            linesWritten++;
            if (linesWritten >= splitSize) {
                // Достигли лимита на количество строк в partition файле
                linesWritten = 0;
                output.close();
                partitionNum++;
                partitionFile = OUTPUT_DIR + OUT_FILE + to_string(partitionNum) + ".txt";
                outputFiles.push_back(partitionFile);
                output.open(partitionFile);
                if (!output.is_open()) {
                    cerr << "Error: Unable to create partition file " << partitionFile << endl;
                    return;
                }
            }
        }
        input.close();
    }
    output.close();
}
