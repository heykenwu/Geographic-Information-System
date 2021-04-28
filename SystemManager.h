#pragma once
#ifndef SYSTEMMANAGER_H
#define SYSTEMMANAGER_H
#include <string>
#include <iostream>
#include <vector>
#include "CommandProcessor.h"
#include "Logger.h"
#include "BufferPool.h"
#include "CoordinateIndex.h"
#include "NameIndex.h"
#include "DMS.h"
#include "GISRecord.h"
using namespace std;

enum Command {
    WORLD,
    IMPORT,
    DEBUG,
    QUIT, 
    WHAT_IS_AT, 
    WHAT_IS, 
    WHAT_IS_IN,
};

class SystemManager {
    private:
        string data_file;
        string script_file;
        string log_file;
        Logger logger;
        vector<string> command_list;
        Command convert_command_type(string x);
        vector<string> seperate_command(string cmd);
    public:
        SystemManager();
        ~SystemManager();
        SystemManager(int arc, char *argv[]);
        void parse_commands();
        void database_log_creation();
        void debug(vector<string> arr);
        string import_str(int name_size,int max, int lo, int average_name);
};

#endif