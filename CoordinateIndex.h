#pragma once
#ifndef COORDINATEINDEX_H
#define COORDINATEINDEX_H
#include <string>
#include <iostream>
#include <vector>
#include <utility>
using namespace std;
class Record_Location{
    int latitude;
    int longitude;
    public:
        Record_Location(){}
        Record_Location(int l, int lo){
            latitude = l;
            longitude = lo;
        }
        void set_latitude(int x){
            latitude = x;
        }
        void set_longitude(int x){
            longitude = x;
        }

        int get_lat(){
            return latitude;
        }

        int get_long(){
            return longitude;
        }

        bool same(Record_Location tmp_rd){
            if((latitude == tmp_rd.latitude) && (longitude == tmp_rd.longitude)){
                return true;
            }
            return false;
        }
};

class Node{
    Record_Location location;
    vector<int> offsets;
    public:
        Node(){};
        void setLocation(int l, int lo) {
            location = Record_Location(l,lo);
        }
        void add_offset(int offset) {
            offsets.push_back(offset);
        }

        Record_Location get_location(){
            return location;
        }

        vector<int> get_offsets(){
            return offsets;
        }
};

class tree{
    private:
        int latitude_south;
        int latitude_north;
        int longitude_west;
        int longitude_east;
        Node *root;
        tree *top_left;
        tree *top_right;
        tree *bot_left;
        tree *bot_right;
    public:
        tree(){};
        tree(int la_start,int la_end,int long_start, int long_end);
        void insert(Record_Location record, vector<int> offset);
        vector<int> search_single(int latitude, int longitude);
        bool inBoundary(int,int);
        string print_Util(string ,tree*,int);
        string print(tree* root);
        tree search_by_range_Util(int la, int lo, int la_range, int lo_range);
        void search_by_range(int la, int lo, int la_range, int lo_range, tree* subtree, vector<int> &vec);
        void get_size(tree*,int&);
};
class CoordinateIndex{
    private:
        tree quadtree;
    public:
        CoordinateIndex(){};
        CoordinateIndex(int,int,int,int);
        void insert(int l, int lo, vector<int> offset);
        vector<int> search_single(int latitude, int longitude);
        string print();
        vector<int> search_in_range(int la, int lo, int la_range, int lo_range);
        int get_size();
};

#endif