#include "NameIndex.h"

unsigned int NameIndex::elfhash(string key){
    unsigned int h = 0;
    unsigned int s;

    for (int i =0; i < key.length(); i++) {
        h = (h << 4) + key[i];
        s = h & 0xf0000000L;
        if(s) {
            h ^= s >> 24;
        }

        h &= ~s;
    }
    return h;
}

void NameIndex::expandAndRehash(){
        c*=2;
        size = 0;
        avg_name_len = 0;
        vector<pair<string,vector<int>>> tmp_buckets(c);
        vector<BucketStatus> tmp_status(c);       
        for (int g = 0; g < buckets.size(); g++)
        {
            tmp_buckets[g] = buckets[g];
            tmp_status[g] = status[g];
        }
        pair<string,vector<int>> re_;
        buckets.assign(c,re_);
        status.assign(c,EMPTY);
        for (int i = 0; i < tmp_buckets.size(); i++)
        {
            if (tmp_status[i] == OCCUPIED)
                for(int k = 0; k < tmp_buckets[i].second.size(); k ++) {
                    insert(tmp_buckets[i].first,tmp_buckets[i].second[k]);
                }
        }
}

bool NameIndex::insert(string name_state, int offset)
{
    if (static_cast<double>(size) / c >= 0.7)
    {
        expandAndRehash();
    }
    int numCollisions = 0;
    unsigned int h = elfhash(name_state) % c;
    unsigned int i = 0;
    unsigned int hi = h;
    unsigned int probe;
    while (status[hi] == OCCUPIED)
    {
        if (buckets[hi].first == name_state){
            if(!(std::find(buckets[hi].second.begin(), buckets[hi].second.end(), offset) != buckets[hi].second.end())) {
                buckets[hi].second.push_back(offset);
            }
            return true;
        }
        ++i;
        probe = (i*i + i) / 2;    
        numCollisions++;
        hi = (hi + probe) % c;
    }
    avg_name_len+=name_state.length()-2;
    status[hi] = OCCUPIED;
    buckets[hi].first = name_state;
    buckets[hi].second.push_back(offset);
    size++;
    if(numCollisions > max_numCollisions) {
        max_numCollisions = numCollisions;
    }
    return true; // Key inserted successfully
}

pair<string,vector<int>> NameIndex::search(string name_state)
{
    unsigned int h = elfhash(name_state) % c;
    unsigned int i = 0;
    unsigned int hi = h;
    unsigned int probe;
    while (status[hi] == OCCUPIED)
    {
        if (buckets[hi].first == name_state)
        {
            return buckets[hi];
        }
        ++i;
        probe = (i*i + i) / 2; 
        hi = (hi + probe) % c;
    }
    pair<string,vector<int>> not_found;
    not_found.first = "null";
    // Key not found. Hit an empty bucket.
    return not_found;
}

string NameIndex::str(){
    string result = "";
    result += "Format of display is\n";
    result +="Slot number: data record\n";
    result +="Current table size is " + to_string(c) +"\n";
    result +="Number of elements in table is " + to_string(size) + "\n\n";
    for(int i = 0; i < buckets.size(); i++){
        if(status[i] == OCCUPIED) {
            result+= "  "+to_string(i);
            result+=": [";
            result+=buckets[i].first.substr(0,buckets[i].first.length() - 2) +":";
            result+=buckets[i].first.substr(buckets[i].first.length() - 2,2) + ", [";
            result+= to_string(buckets[i].second[0]);
            for(int j = 1; j<buckets[i].second.size();j++) {
                 result+= ","+to_string(buckets[i].second[j]);
            }
            result+= "]]\n";
        }
    }
    return result;
}

int NameIndex::name_length(){
    return (avg_name_len/size);
}

int NameIndex::longest_probe() {
    return max_numCollisions;
}

int NameIndex::size_by_name() {
    return size;
}