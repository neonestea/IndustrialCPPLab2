#pragma once

#include "specification.hpp"
#include "shuffler.hpp"

#include <chrono>
//#include <algorithm>
#include <map>
//#include <queue>
#include <vector>
#include <thread>
#include <atomic>
#include "directory_source.hpp"
#include "map.hpp"
#include "storage.hpp"
#include "reduce.hpp"
#include <string>
#include <mutex>
#include <condition_variable>

class Synchronization {
public:
    Synchronization(const std::size_t num_threads) : 
      _num_threads(num_threads) {
    }

    void synch() {
        std::unique_lock lock(m);
        ++thread_cnt;
        if (thread_cnt >= _num_threads) {
            cv.notify_all();
        } else {
            while (thread_cnt < _num_threads) {
                cv.wait(lock);
            }
        }
        ++thread_out;
        if (thread_out == _num_threads) {
            thread_cnt = 0;
            thread_out = 0;
        }
    }

private:
    std::atomic_int thread_out{0};
    std::atomic_int thread_cnt{0};
    const size_t _num_threads = 0;
    std::mutex m;
    std::condition_variable cv;
};

void run_map_phase(std::string& str, Synchronization& synchron, Map& map_fn, Storage& istore)
{
    map_fn.map(str, istore);
    synchron.synch();
}

void run_shuffler_phase(Synchronization& synchron, Storage& istore, std::vector<Storage>& shuffled, int reducers_count, Shuffler& shuffler, int id)
{
    shuffler.shuffle(istore, shuffled, reducers_count, id);
    synchron.synch();
}

void run_reducer_phase(Storage& shuffled, int id)
{
    Reduce fn;
    fn.reduce(shuffled, id);
}

class Job
{
public:

    Job(DirectorySource& ds, Map& map_fn, Storage& is, Shuffler& cfn, Reduce& reduce_fn, Storage& output_store)
        : input_ds(ds), map_fn(map_fn), shuffler(cfn), istore(is), reduce_fn(reduce_fn), output_store(output_store)
    {
    }

    ~Job()
    {
    }

    void start()
    {
        int map_workers = 4; //заменить!
        
        Synchronization synchronizatorMap(map_workers); //не нужен
        
        std::vector<std::string> files;                                                                       //заменить !
        files.push_back("../../projects/src/dummyfiles/file1.txt");
        files.push_back("../../projects/src/dummyfiles/file2.txt");
        files.push_back("../../projects/src/dummyfiles/file3.txt");
        files.push_back("../../projects/src/dummyfiles/file4.txt");

        std::vector<std::thread> map_threads = {};
        for (size_t i = 0; i < map_workers; ++i) {
            std::thread map_thread(run_map_phase, std::ref(files[i]), std::ref(synchronizatorMap), std::ref(map_fn), std::ref(istore));
            map_threads.emplace_back(std::move(map_thread));
        }

        for (size_t i = 0; i < map_workers; ++i) {
            map_threads[i].join();
        }
        size_t reducers_count = istore.get_amount();
        unsigned int n = std::thread::hardware_concurrency();
        if (reducers_count > n) {
            reducers_count = n;
        }
        Synchronization synchronizatorShuffler(reducers_count); // Не нужен
        std::vector<Storage> shuffled;
        std::vector<std::thread> shuffle_threads = {};
        for (size_t i = 0; i < reducers_count; ++i) {
            int thread_id = i;
            std::thread shuffle_thread(run_shuffler_phase, std::ref(synchronizatorShuffler), std::ref(istore), std::ref(shuffled), std::ref(reducers_count), std::ref(shuffler), thread_id);
            shuffle_threads.emplace_back(std::move(shuffle_thread));
        }
        for (size_t i = 0; i < reducers_count; ++i) {
            shuffle_threads[i].join();
        }

        std::vector<std::thread> reducer_threads = {};

        for (size_t i = 0; i < reducers_count; ++i) {
            int thread_id = i;
            std::thread reducer_thread(run_reducer_phase, std::ref(shuffled[i]), thread_id);
            reducer_threads.emplace_back(std::move(reducer_thread));
        }

        for (size_t i = 0; i < reducers_count; ++i) {
            reducer_threads[i].join();
        }
    }

private:
    using map_task_inputs_t = std::map<std::string, std::vector<std::string>>;

private:
    // note that each process has its own copy
    DirectorySource& input_ds;
    Map& map_fn;
    Storage& istore; // accumulates intermediates from successive tasks
    Shuffler& shuffler;
    Reduce& reduce_fn;
    Storage& output_store;

    enum {
        MapPhaseBegin,
        MapTaskAssignment,
        MapTaskCompletion,
        MapPhasePing,
        MapPhaseEnd,

        ShufflePhaseBegin,
        ShuffleIntermediateCounts,
        ShuffleDistributionMap,
        ShufflePayloadDelivery,
        ShufflePayloadDeliveryComplete,
        ShufflePhaseEnd,

        GatherPayloadDelivery,
        GatherPayloadDeliveryComplete
    };

    struct TaskItem {
        map_task_inputs_t inputs;

        struct {
            int worker;
            bool completed;
            std::chrono::time_point<std::chrono::steady_clock> start_time, end_time;
            std::chrono::time_point<std::chrono::steady_clock> last_ping_time;
        } status;
    };
    
    // indices are task id
    std::vector<TaskItem> map_tasks;
    std::vector<int> failed_workers;
};
