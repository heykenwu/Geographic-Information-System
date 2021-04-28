#pragma once
#ifndef DMS_H
#define DMS_H
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <vector>
using namespace std;
struct DMS {
    int degree;
    int minute;
    int second;
    char hemisphere;
    void setLogitude(string l) {
        if (l.length() != 8) {
            cout << "setlongitude error" << endl;
            exit(4);
        }
        degree = stoi(l.substr(0,3));
        minute = stoi(l.substr(3,2));
        second = stoi(l.substr(5,2));
        hemisphere = l[7];
    }
    void setlatitude(string l) {
        if (l.length() != 7) {
            cout << "setlatitude error" << endl;
            exit(5);
        }
        degree = stoi(l.substr(0,2));
        minute = stoi(l.substr(2,2));
        second = stoi(l.substr(4,2));
        hemisphere = l[6];
    }
    int get_DMS_in_sec(){
        int result = degree * 3600;
        result += minute * 60;
        result += second;
        if((hemisphere == 'W') || hemisphere == 'S') {
            result *= -1;
        }
        return result;
    }
    string get_string(){
        string result = to_string(degree)+"d " + to_string(minute)+"m " + to_string(second) +"s ";
        if(hemisphere == 'W') 
        {
            result+="West";
        } else if (hemisphere == 'E')
        {
            result+="East";
        } else if (hemisphere == 'S')
        {
            result+="South";
        } else if (hemisphere == 'N')
        {
            result+="North";
        }
        
        return result;
    }
};

#endif