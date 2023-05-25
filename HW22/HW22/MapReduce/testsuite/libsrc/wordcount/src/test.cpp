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
    fs::path tmp = "../../data/output/";
    std::uintmax_t n = fs::remove_all(tmp);

    std::string source_dir = "../../data/input";
    Map mapfn;
    Shuffler shuffler;
    Storage intermediate_store;
    Reduce reducefn;
    Storage output_store;
    Job job(mapfn, intermediate_store, shuffler, reducefn, output_store);
    job.start(); //запуск
    REQUIRE(intermediate_store.checkPair("And") == 2); //что сделал маппер
    REQUIRE(intermediate_store.checkPair("As") == 1);
    REQUIRE(intermediate_store.checkPair("and") == 5);
    REQUIRE(intermediate_store.checkPair("be") == 2);
    REQUIRE(intermediate_store.checkPair("constantly") == 2);
    REQUIRE(intermediate_store.checkPair("full") == 3);
    REQUIRE(intermediate_store.checkPair("in") == 5);
    REQUIRE(intermediate_store.checkPair("like") == 2);
    REQUIRE(intermediate_store.checkPair("skies") == 1);
    REQUIRE(intermediate_store.checkPair("stare") == 3);
    REQUIRE(intermediate_store.checkPair("time") == 8);
    REQUIRE(intermediate_store.checkPair("Safarli") == 1);
    REQUIRE(intermediate_store.checkPair("way") == 1);
    REQUIRE(intermediate_store.checkPair("to") == 8);
    REQUIRE(intermediate_store.checkPair("be") == 2);
    REQUIRE(intermediate_store.checkPair("maintained") == 1);
    REQUIRE(intermediate_store.checkPair("full") == 3);
    REQUIRE(intermediate_store.checkPair("their") == 1);
    REQUIRE(intermediate_store.checkPair("your") == 1);
    REQUIRE(intermediate_store.checkPair("idempotential") == 0);

    //intermediate_store.print();
    REQUIRE(intermediate_store.get_amount() == 124);
}


TEST_CASE("MapReduce", "[check_out_0.txt]")
{
    std::ifstream f("../../data/output/out_0.txt");
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
    std::ifstream f("../../data/output/out_1.txt");
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
