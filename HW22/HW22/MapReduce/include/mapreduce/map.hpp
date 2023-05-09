#pragma once
#include <string>
#include <iostream>
#include <sstream>
#include <boost/range/istream_range.hpp>
#include "storage.hpp"

class Map {
public:
    void map(std::string filename, Storage& store) {
        std::ifstream ifs(filename);
        //std::cout << "Key " << filename << std::endl;
                                
        for (const auto& word : boost::range::istream_range<std::string>(ifs))
        {
            //std::cout << "word " << word << std::endl;
            if (word.size() > 100)
                continue;
            store.pushback(word, 1);
        }
    }
};