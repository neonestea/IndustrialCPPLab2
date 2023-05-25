#pragma once
#include <string>
#include <iostream>
#include <sstream>
#include <boost/range/istream_range.hpp>
#include "storage.hpp"
#include "multithread_vector.hpp"
#include <vector>

class Map {
public:
    std::vector<std::string> split(const std::string &s, char delim) {
    std::stringstream ss(s);
    std::string item;
    std::vector<std::string> elems;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
        return elems;
    }

    void map(/*std::string filename,*/ Storage& store, int id, LockVector<std::string>& filenames, int batch_size, int total) {
        for (int i = id; i < total; i += batch_size ) {
            std::ifstream f(filenames[i]);
            std::string line;
        
            while (std::getline(f, line)) {
                //std::cout << "line " << line << std::endl;
                std::vector<std::string> words = split(line, ' ');
                for (const auto& word : words)
                {
                    //std::cout << "word " << word << std::endl;
                    //if (word.size() > 100)
                    //    continue;
                    store.pushback(word, 1);
                }
                
            }
            f.close();


           // std::ifstream ifs();
            //std::cout << "Key " << filename << std::endl;
                                    
           // for (const auto& word : boost::range::istream_range<std::string>(ifs))  //too clever
           // {
            //    std::cout << "word " << word << std::endl;
            //    if (word.size() > 100)
           //         continue;
           //     store.pushback(word, 1);
          //  }//
        }
    }
};
