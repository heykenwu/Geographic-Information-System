#include "Logger.h"

Logger::Logger(){};

Logger::~Logger(){};

Logger::Logger(string file){
    log_file.open(file,std::ofstream::app);
}

void Logger::write_comment(string st){
    log_file << st << endl;
}

void Logger::write_command(int no, string st){
    log_file << "Command " << to_string(no) << ": " << st << endl;
}

void Logger::write_endline(){
    log_file << "------------------------------------------------------------------------------------------" << endl;
}

void Logger::write_header(string db, string script, string log){
    log_file << "Course Project for COMP 8042" << endl;
    log_file << "Student Name: Hong Kit Wu, Student Id: A00968591" << endl;
    log_file << "Begin of GIS Program log:" << endl;
    log_file << "dbFile:\t"  << db << endl;
    log_file << "script:\t" << script << endl;
    log_file << "log:\t" << log << endl;
    write_time(1);
}
void Logger::write_time(int x){
    time_t rawtime;
    struct tm * timeinfo;
    char buffer [256];
    time (&rawtime);
    timeinfo = localtime (&rawtime);
    if(x == 1) {
        strftime (buffer,256,"Start time: %a %b %d %T %Z %Y",timeinfo);       
    } else {
        strftime (buffer,256,"End time: %a %b %d %T %Z %Y",timeinfo);
    }
    log_file << buffer << endl;
}

void Logger::write_world(int wl,int el, int sl, int nl){
    log_file << "------------------------------------------------------------------------------------------" << endl;
    log_file << "Latitude/longitude values in index entries are shown as signed integers, in total seconds." << endl;
    log_file << "------------------------------------------------------------------------------------------" << endl;
    log_file << "            World boundaries are set to:" << endl;
    log_file << "                       " << to_string(nl) << endl;
    log_file << "            " << to_string(wl) << "              " << to_string(el) << endl;
    log_file << "                       " << to_string(sl) << endl;
}
void Logger::close() {
    log_file.close();
}
