#pragma once

#include "storage.hpp"
#include <thread>
#include <filesystem>
#include <iostream>

const std::string OUT_DIR = "../../projects/src/output/"; // Next to dummyfiles/
const std::string OUT_FILE = "out_"; // out_0.txt out_1.txt ...

std::string GetReduceFileName(const std::string& type, uint64_t file_num) {
    return OUT_DIR + type + std::to_string(file_num) + ".txt";
}

void result_dir(std::string finalpath)
{
    if (!std::filesystem::is_directory(finalpath) || !std::filesystem::exists(finalpath))
        std::filesystem::create_directory(finalpath); // Create folder
    if (!std::filesystem::exists(finalpath.c_str()))
       	    std::cout << "Cannot create " << finalpath << " folder error: " << strerror(errno) << std::endl;
}

class Reduce{
public:
    void reduce(Storage& store, int id) {
        result_dir(OUT_DIR);
        const std::string file_name = GetReduceFileName(OUT_FILE, id);
        std::ofstream output(file_name.c_str());
        for (int i = 0; i < store.get_amount(); ++i) {
            auto res = store.get_ith(i);
            if (res) {
                auto taken = std::move(*res);
                
                size_t count = 0;
                for (auto val : taken.second) {
                    count += val;
                }
                output << taken.first << "\t" << count << std::endl;
            }
        }
        output.close();
    }
};
