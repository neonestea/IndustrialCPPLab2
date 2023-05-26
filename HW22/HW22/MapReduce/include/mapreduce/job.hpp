#pragma once

#include "shuffler.hpp"

#include <chrono>
//#include <algorithm>
#include <map>
//#include <queue>
#include <vector>
#include <thread>
#include <atomic>
#include "map.hpp"
#include "storage.hpp"
#include "reduce.hpp"
#include <string>
#include <mutex>
#include <condition_variable>
#include "multithread_vector.hpp"
#include "splitter.hpp"

std::vector<std::vector<char>> SplitVector(const std::vector<char>& vec, size_t n)
{
    std::vector<std::vector<char>> outVec;

    size_t length = vec.size() / n;
    size_t remain = vec.size() % n;

    size_t begin = 0;
    size_t end = 0;

    for (size_t i = 0; i < std::min(n, vec.size()); ++i)
    {
        end += (remain > 0) ? (length + !!(remain--)) : length;

        outVec.push_back(std::vector<char>(vec.begin() + begin, vec.begin() + end));

        begin = end;
    }

    return outVec;
}


void run_map_phase(/*std::string& str, Synchronization& synchron, */Map& map_fn, Storage& istore, int id, LockVector<std::string>& files, int batch_size, int total)
{
    map_fn.map(/*str,*/ istore, id, files, batch_size, total);
    //synchron.synch();
}

void run_shuffler_phase(/*Synchronization& synchron,*/ Storage& istore, LockVector<Storage>& shuffled, int reducers_count, Shuffler& shuffler, int id, std::vector<char>& chars)
{
    
    shuffler.shuffle(istore, shuffled, reducers_count, id, chars);
    //synchron.synch();
}

void run_reducer_phase(Storage& shuffled, int id)
{
    Reduce fn;
    fn.reduce(shuffled, id);
}

class Job
{
public:

    Job(Map& map_fn, Storage& is, Shuffler& cfn, Reduce& reduce_fn, Storage& output_store)
        : map_fn(map_fn), shuffler(cfn), istore(is), reduce_fn(reduce_fn), output_store(output_store)
    {
    }

    ~Job()
    {
    }

    void start()
    {
       
        unsigned int n = std::thread::hardware_concurrency();
        


        std::vector<std::string> inputFiles;
        for (int i = 0; i < 5; ++i)
            inputFiles.push_back(OUTPUT_DIR + "/" +"file" + to_string(i) + ".txt");
        /*inputFiles.push_back("../../data/input/file1.txt");
        inputFiles.push_back("../../data/input/file2.txt");
        inputFiles.push_back("../../data/input/file3.txt");
        inputFiles.push_back("../../data/input/file4.txt");
        inputFiles.push_back("../../data/input/file5.txt");*/
        std::string outputDir = "../../data/input/";
        double splitPercent = 0.2;
        LockVector<string> outputFiles;
        splitFiles(inputFiles, outputDir, splitPercent, outputFiles);
        
        for (int i=0; i<outputFiles.size(); ++i){
            std::cout<<outputFiles[i]<<std::endl;
        }

        int batch_size = outputFiles.size();

        if (batch_size > n) {
            batch_size = n;
        }
        int map_workers = batch_size;

        std::vector<std::thread> map_threads = {};


        for (int i = 0; i < map_workers; ++i) {

            int thread_id = i;

            std::thread map_thread(run_map_phase, /*std::ref(files[i]), std::ref(synchronizatorMap),*/ std::ref(map_fn), std::ref(istore),  thread_id, std::ref(outputFiles), batch_size, outputFiles.size());

            map_threads.emplace_back(std::move(map_thread));
        }

        for (int i = 0; i < map_workers; ++i) {
            map_threads[i].join();
        }
        //Mapping completed
//        istore.print();
        size_t reducers_count = istore.get_amount();
       // unsigned int n = std::thread::hardware_concurrency();
        if (reducers_count > n) {
            reducers_count = n;
        }

        std::vector<char> charVector = {'A', 'a', 'B', 'b', 'C', 'c', 'D', 'd', 'E', 'e', 'F', 'f', 'G', 'g', 'H', 'h', 'I', 'i', 'J', 'j', 'K', 'k', 'L', 'l', 'M','m', 'N', 'n', 'O', 'o', 'P', 'p',
         'Q', 'q', 'R', 'r', 'S','s', 'T', 't', 'U', 'u', 'V', 'v', 'W','w', 'X', 'X', 'Y', 'y', 'Z', 'z'};
        std::vector<std::vector<char>> charVectors = SplitVector(charVector, reducers_count);

        for (auto vec : charVectors){
            std::cout << "New vector";
            std::cout << '\n';
            for (auto letter : vec){
                std::cout << letter << ' ';
            }
            std::cout << '\n';
        }
        
        
        
        //Synchronization synchronizatorShuffler(reducers_count); // Не нужен
        LockVector<Storage> shuffled;
        std::vector<std::thread> shuffle_threads = {};
//        std::cout << "\n\n\n\n";
//        std::cout << "Shuffling " << reducers_count << " threads" << std::endl;
        for (size_t i = 0; i < reducers_count; ++i) {
            int thread_id = i;
            std::thread shuffle_thread(run_shuffler_phase, /*std::ref(synchronizatorShuffler), */std::ref(istore), std::ref(shuffled), std::ref(reducers_count), std::ref(shuffler), thread_id, std::ref(charVectors[i]));
            shuffle_threads.emplace_back(std::move(shuffle_thread));
        }
        for (size_t i = 0; i < reducers_count; ++i) {
            shuffle_threads[i].join();
        }
/*
        std::cout << "\n\n\n\n";
        for (int i = 0; i < shuffled.size(); ++i) {
            std::cout << "New batch #" << i << std::endl;
            shuffled[i].print();
        }
*/
        //Shuffling completed!
        std::vector<std::thread> reducer_threads = {};

//        std::cout << "Reducing " << reducers_count << " threads" << std::endl;
        for (size_t i = 0; i < reducers_count; ++i) {
            int thread_id = i;
            std::thread reducer_thread(run_reducer_phase, std::ref(shuffled[i]), thread_id);
            reducer_threads.emplace_back(std::move(reducer_thread));
        }

        for (size_t i = 0; i < reducers_count; ++i) {
            reducer_threads[i].join();
        }
//        std::cout << "\n\n\n\n";

        
    }

private:
    
    Map& map_fn;
    Storage& istore; // accumulates intermediates from successive tasks
    Shuffler& shuffler;
    Reduce& reduce_fn;
    Storage& output_store;
};
