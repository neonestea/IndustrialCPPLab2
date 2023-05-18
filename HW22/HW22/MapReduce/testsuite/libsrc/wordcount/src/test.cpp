#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_all.hpp>

#include "job.hpp"
#include "map.hpp"
#include "shuffler.hpp"
#include "reduce.hpp"
#include "storage.hpp"
#include <iostream>
#include <fstream>
#include <thread>
#include <vector>
#include <string>
#include <sstream>
#include <numeric>
#include <filesystem>


TEST_CASE("MapReduce", "[rebuild_output]")
{
    namespace fs = std::filesystem;
    fs::path tmp = "../../projects/src/output/";
    std::uintmax_t n = fs::remove_all(tmp);

    std::string source_dir = "../../projects/src/dummyfiles";
    Map mapfn;
    Shuffler shuffler;
    Storage intermediate_store;
    Reduce reducefn;
    Storage output_store;
    Job job(mapfn, intermediate_store, shuffler, reducefn, output_store);
    job.start(); //запуск
}


TEST_CASE("MapReduce", "[check_out_0.txt]")
{
    std::ifstream f("../../projects/src/output/out_0.txt");
    std::string line;
 
    while (std::getline(f, line))
    {
        std::istringstream ss(line);
 
        std::string key;
        std::getline(ss, key, '\t');
        std::string value;
        std::getline(ss, value, '\t');
        if(key.compare("file1") == 0)
            REQUIRE(std::stoi(value) == 3 );
        else if(key.compare("file3") == 0)
            REQUIRE(std::stoi(value) == 2);
        else if(key.compare("hello") == 0)
            REQUIRE(std::stoi(value) == 4);
    }
    f.close();
}

TEST_CASE("MapReduce", "[check_out_1.txt]")
{
    std::ifstream f("../../projects/src/output/out_1.txt");
    std::string line;
 
    while (std::getline(f, line))
    {
        std::istringstream ss(line);
 
        std::string key;
        std::getline(ss, key, '\t');
        std::string value;
        std::getline(ss, value, '\t');
        if(key.compare("file2") == 0)
            REQUIRE(std::stoi(value) == 1);
        else if(key.compare("from") == 0)
            REQUIRE(std::stoi(value) == 4);
    }
    f.close();
}
