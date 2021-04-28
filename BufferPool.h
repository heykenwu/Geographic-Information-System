#pragma once
#ifndef BUFFERPOOL_H
#define BUFFERPOOL_H
#include <string>
#include <iostream>
#include <vector>
#include <utility>
#include "GISRecord.h"
#include "NameIndex.h"
#include "DMS.h"
#include "CoordinateIndex.h"
#include <fstream>
#include <sstream>

using namespace std;

class BufferPool {
    private:
        string database_file;
        vector<pair<GISRecord,int>> lru_array;
        vector<int> file_offset;
        int buffer_size = 15;
        void sort_buffer();
    public:
        BufferPool(){};
        BufferPool(string db_file) : database_file{db_file} {};
        void add(GISRecord record, int offset);
        string search_by_name_state(string name_state, NameIndex nameIndex);
        vector<GISRecord> get_data(vector<int> offsets);
        pair<string,GISRecord> is_contain(int offset);
        string str();
        string what_is_at(vector<string>,CoordinateIndex coord);
        string what_is_in(vector<string>,CoordinateIndex coord);
};
#endif