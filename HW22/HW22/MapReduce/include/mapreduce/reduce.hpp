#pragma once

#include "storage.hpp"
#include <thread>
#include <sys/stat.h>
#include <filesystem>

const std::string OUT_DIR = "../../projects/src/output/";
const std::string OUT_FILE = "out"; // out0.txt out1.txt ...

std::string GetReduceFileName(const std::string& type, uint64_t file_num) {
    return OUT_DIR + type + std::to_string(file_num) + ".txt";
}

int result_dir(std::string finalpath){
    int ret = 0;
    if (mkdir(finalpath.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == -1) {
        if( errno == EEXIST ) { // Alredy exists
        }
        else { // Something else
       	    std::cout << "Cannot create " << finalpath.c_str() << " folder error: " << strerror(errno) << std::endl;
            ret = 1;
        }
    }
    return ret;
}

class Reduce{
public:
    void reduce(Storage& store, int id) {
        if( result_dir(OUT_DIR) == 0) {
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
        else
            std::cout << "Cannot write to directory " << OUT_DIR << std::endl;
    }
};
