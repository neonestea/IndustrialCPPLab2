#include "job.hpp"
#include "map.hpp"
#include "shuffler.hpp"
#include "reduce.hpp"
#include "specification.hpp"
#include "storage.hpp"
#include "directory_source.hpp"

#include <iostream>
#include <fstream>
#include <thread>
#include <vector>
#include <string>
#include <sstream>
#include <numeric>


int main (int argc, char* argv[])
{
    std::string source_dir = "../../projects/src/dummyfiles";
    DirectorySource datasource(source_dir);
    Map mapfn;
    Shuffler shuffler;
    Storage intermediate_store;
    Reduce reducefn;
    Storage output_store;

    Job job(datasource, mapfn, intermediate_store, shuffler, reducefn, output_store);

    
    
    job.start(); //запуск
    
    return 0;
}