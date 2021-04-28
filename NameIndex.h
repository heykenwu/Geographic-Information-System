#pragma once
#ifndef NAMEINDEX_H
#define NAMEINDEX_H
#include <string>
#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
// #include "BufferPool.h"
using namespace std;

enum BucketStatus
{
    EMPTY,
    OCCUPIED
};

class NameIndex {
    private:
        /// The total number of collisions since the creation of the hashfunction.
        int max_numCollisions;
        // totla number of occupied buckets
        int size;
        /// Capacity of the hashtable
        int c;
        /// All the buckets in the hashtable
        vector<pair<string,vector<int>>> buckets;
        /// Status of all the buckets
        vector<BucketStatus> status;

        int avg_name_len=0;
    public:
        NameIndex(): max_numCollisions(0), size(0) {
            c=1024;
            buckets.resize(c);
            status.resize(c);
        };
        unsigned int elfhash(string key);
        void expandAndRehash();
        bool insert(string name_state, int offset);
        pair<string,vector<int>> search(string name_state);
        string str();
        int name_length();
        int longest_probe();
        int size_by_name();
};

#endif