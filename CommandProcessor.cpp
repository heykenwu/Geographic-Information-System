#include "CommandProcessor.h"

CommandProcessor::CommandProcessor(){};

CommandProcessor::~CommandProcessor(){};

vector<string> CommandProcessor::load_file(string file_name){

    ifstream fp(file_name);
    string line;
    vector<string> cmd_list;
    while (getline(fp, line)) {
        if(line.empty()) {
            continue;
        }
        cmd_list.push_back(line);
    }
    return cmd_list;
}

vector<DMS> CommandProcessor::world(vector<string> arg) {
    if(arg.size() != 5) {
        cout << "world function error !" << endl;
        exit(3);
    }
    DMS w_long,e_long,s_lat,n_lat;
    w_long.setLogitude(arg[1]);
    e_long.setLogitude(arg[2]);
    s_lat.setlatitude(arg[3]);
    n_lat.setlatitude(arg[4]);

    vector<DMS> result;
    result.push_back(w_long);
    result.push_back(e_long);
    result.push_back(s_lat);
    result.push_back(n_lat);

    return result;
}

void CommandProcessor::import_database(string newdb, string data_file) {
    string line;
    std::ifstream  src(newdb, std::ios::binary);
    std::ofstream  dst(data_file,std::ios::binary | std::ofstream::out);
    dst << src.rdbuf();
    src.close();
    dst.close();
}

NameIndex CommandProcessor::name_hashtable_import(string data_file,vector<DMS> world) {
    NameIndex ni = NameIndex();
    std::ifstream  fp(data_file);
    string line;
    getline(fp,line);
    int offset = fp.tellg().operator std::streamoff();
    while (getline(fp,line)){
        GISRecord tmp = tmp.parse_GIS_raw(line);
        if(tmp.get_primary_latitude_DMS() == "Unknown" || tmp.get_primary_longitude_DMS() == "Unknown") {
            offset = fp.tellg().operator std::streamoff();
            continue;
        }
        DMS lat,lot;
        lat.setlatitude(tmp.get_primary_latitude_DMS());
        lot.setLogitude(tmp.get_primary_longitude_DMS());
        if((lat.get_DMS_in_sec() >= world[2].get_DMS_in_sec())&& (lat.get_DMS_in_sec() <= world[3].get_DMS_in_sec()) && (lot.get_DMS_in_sec() >= world[0].get_DMS_in_sec()) && (lot.get_DMS_in_sec() <= world[1].get_DMS_in_sec())) {
            string name_state = tmp.get_feature_name()+tmp.get_state_alpha();
            ni.insert(name_state,offset);
        }
        offset = fp.tellg().operator std::streamoff();
    }
    return ni;
}

CoordinateIndex CommandProcessor::coord_import(string data_file,int ls,int ln,int lw,int le) {
    CoordinateIndex ci = CoordinateIndex(ls,ln,lw,le);
    std::ifstream  fp(data_file);
    vector<int> offsets;
    string line;
    getline(fp,line);
    int offset = fp.tellg().operator std::streamoff();
    offsets.push_back(offset);
    while (getline(fp,line)){
        GISRecord tmp = tmp.parse_GIS_raw(line);
        if(tmp.get_primary_latitude_DMS() == "Unknown" || tmp.get_primary_longitude_DMS() == "Unknown") {
            offset = fp.tellg().operator std::streamoff();
            continue;
        }
        DMS latitude,longitude;
        latitude.setlatitude(tmp.get_primary_latitude_DMS());
        longitude.setLogitude(tmp.get_primary_longitude_DMS());
        ci.insert(latitude.get_DMS_in_sec(),longitude.get_DMS_in_sec(),offsets);
        offsets.clear();
        offset = fp.tellg().operator std::streamoff();
        offsets.push_back(offset);
    }
    return ci;
}

string CommandProcessor::whatIsAt(vector<string> cmd, BufferPool &bf_pool,CoordinateIndex coord) {

    return bf_pool.what_is_at(cmd,coord);
}

string CommandProcessor::debug_pool(BufferPool bf) {

    return bf.str();
}

string CommandProcessor::whatIsIn(vector<string> cmd, BufferPool &bf_pool,CoordinateIndex coord) {
    return bf_pool.what_is_in(cmd,coord);
}