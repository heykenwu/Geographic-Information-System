#pragma once
#ifndef LOGGER_H
#define LOGGER_H
#include <string>
#include <iostream>
#include <fstream>
#include <iterator>
#include <vector>
#include <sstream>
#include <time.h>

using namespace std;

class Logger {
    private:
        ofstream log_file;
    public:
        Logger();
        ~Logger();
        Logger(string file);
        void write_header(string db, string script, string log);
        void write_comment(string st);
        void write_command(int no, string st);
        void write_endline();
        void write_time(int x);
        void write_world(int wl,int el, int sl, int nl);
        void close();
};
#endif