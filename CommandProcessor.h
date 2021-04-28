#pragma once
#ifndef COMMANDPROCESSOR_H
#define COMMANDPROCESSOR_H
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <vector>
#include "NameIndex.h"
#include "GISRecord.h"
#include "CoordinateIndex.h"
#include "BufferPool.h"
using namespace std;

class CommandProcessor {
    public:
        CommandProcessor();
        ~CommandProcessor();
        vector<string> load_file(string file_name);
        vector<DMS> world(vector<string> arg);
        void import_database(string newdb, string data_file);
        NameIndex name_hashtable_import(string data_file,vector<DMS> world);
        CoordinateIndex coord_import(string data_file,int,int,int,int);
        string whatIsAt(vector<string> vec, BufferPool &bf, CoordinateIndex ci);
        string debug_pool(BufferPool bf);
        string whatIsIn(vector<string> cmd, BufferPool &bf_pool,CoordinateIndex coord);
};

#endif
