#include "BufferPool.h"

void BufferPool::add(GISRecord record, int offset){
    if(lru_array.size() == 0) {
        pair<GISRecord,int> new_record (record,0);
        lru_array.push_back(new_record);
        file_offset.push_back(offset);
    } else {
        int least_use_index;
        int max = lru_array[0].second;
        bool in_place = false; 
        for(int i = 0; i < lru_array.size(); i++) {
            if(offset == file_offset[i]) {
                lru_array[i].second = 0;
                in_place = true;
            } else {
                lru_array[i].second++;
                if(lru_array[i].second > max) {
                    max = lru_array[i].second;
                    least_use_index = i;
                }
            }
        }
        if((!in_place) && (lru_array.size() < buffer_size)) {
            pair <GISRecord,int> new_record (record,0);
            lru_array.push_back(new_record);
            file_offset.push_back(offset);
        } else if((!in_place) && (lru_array.size() == buffer_size)){
            pair <GISRecord,int> new_record (record,0);
            lru_array[least_use_index] = new_record;
            file_offset[least_use_index] = offset;
        }
    }
}

void BufferPool::sort_buffer(){
    for(int i = 0; i < lru_array.size(); i++) {
        int min = lru_array[i].second;
        int index;
        bool need_replace = false;
        for(int j = i; j < lru_array.size(); j++) {
            if(lru_array[j].second < min) {
                min = lru_array[j].second;
                index = j;
                need_replace = true;
            }
        }
        if(need_replace) {
            pair <GISRecord,int> tmp = lru_array[i];
            lru_array[i] = lru_array[index];
            lru_array[index] = tmp;

            int tmp2 = file_offset[i];
            file_offset[i] = file_offset[index];
            file_offset[index] = tmp2;
        }
    }
}

pair<string,GISRecord> BufferPool::is_contain(int offset){
    pair<string,GISRecord> result;
    for(int i = 0; i <file_offset.size(); i++) {
        if(file_offset[i] == offset) {
            result.first = "yes";
            result.second = lru_array[i].first;
            lru_array[i].second = 0;
            return result;
        }
    }
    result.first = "null";
    return result;
}


string BufferPool::search_by_name_state(string name_state, NameIndex nameIndex) {
    pair<string,vector<int>> search_result = nameIndex.search(name_state);
    string result="";
    if(search_result.first == "null") {
        result+="  No records match \"" + name_state.substr(0,name_state.length() - 2) +"\" and \"";
        result+=name_state.substr(name_state.length() - 2,2)+"\"";
        return result;
    } else {
        pair<string,GISRecord> tmp;
        vector<int> offset = search_result.second;
        for(int i = 0; i < offset.size(); i++) {
            tmp = is_contain(offset[i]);
            if(tmp.first != "null") {
                DMS latitude,longitude;
                latitude.setlatitude(tmp.second.get_primary_latitude_DMS());
                longitude.setLogitude(tmp.second.get_primary_longitude_DMS());
                result+= "  " + to_string(offset[i]) + ":  " + tmp.second.get_country_name()+"  (" ;
                cout<< tmp.second.get_country_name()<< endl;
                result+=latitude.get_string()+", ";
                result+=longitude.get_string()+")\n";
                offset.erase(offset.begin() + i);
            }
        }
        if(!offset.empty()){
            vector<GISRecord> remain_record = get_data(offset);
            for(int i = 0; i < remain_record.size(); i++) {
                DMS latitude,longitude;
                latitude.setlatitude(remain_record[i].get_primary_latitude_DMS());
                longitude.setLogitude(remain_record[i].get_primary_longitude_DMS());
                result+= "\n  " + to_string(offset[i]) + ":  " + remain_record[i].get_country_name()+"  (" ;
                result+=latitude.get_string()+", ";
                result+=longitude.get_string()+")";
            }
        }
    }
    return result;
}


vector<GISRecord> BufferPool::get_data(vector<int> offsets) {
    ifstream infile(database_file);
    vector<GISRecord> data; 
    string line;
    for (int i = 0; i < offsets.size(); i++) {
        infile.clear();
        infile.seekg(offsets[i], ios::beg);
        getline(infile,line);
        GISRecord tmp = tmp.parse_GIS_raw(line);
        add(tmp,offsets[i]);
        data.push_back(tmp);
    }
    return data;
}

string BufferPool::what_is_at(vector<string>cmd,CoordinateIndex coord){
    DMS latitude,longitude;
    latitude.setlatitude(cmd[1]);
    longitude.setLogitude(cmd[2]);
    vector<int> result = coord.search_single(latitude.get_DMS_in_sec(),longitude.get_DMS_in_sec());
    string matching_result="";
    if(!result.empty()){
        matching_result+= "  The following feature(s) were found at (" +  latitude.get_string() +", " + longitude.get_string()+")";
        for(int i = 0; i < result.size(); i++) {
            pair<string,GISRecord> single_record = is_contain(result[i]);
            if(single_record.first=="yes"){
                matching_result += "\n  "+ to_string(result[i]) + ":  \"" + single_record.second.get_feature_name() + "\"  \"" + single_record.second.get_country_name() +"\"  \"" + single_record.second.get_state_alpha() + "\"";
                result.erase(result.begin() + i);
            }
        }
        if(!result.empty()){
            vector<GISRecord> remain_record = get_data(result);
            for(int i = 0; i < remain_record.size(); i++) {
                matching_result += "\n  " + to_string(result[i]) + ":  \"" + remain_record[i].get_feature_name() + "\"  \"" + remain_record[i].get_country_name() +"\"  \"" + remain_record[i].get_state_alpha() + "\"";
            }
        }
    } else {
        matching_result += " Nothing was found at (\"" + latitude.get_string()+"\", \"" + longitude.get_string()+"\")";
    }
    return matching_result;
}

string BufferPool::what_is_in(vector<string> cmd,CoordinateIndex coord){
    string matching_result="";
    if(cmd.size() >= 5) {
        DMS latitude,longitude;
        int la_range,lo_range;
        string structure="";
        if(cmd.size() == 5) {
            latitude.setlatitude(cmd[1]);
            longitude.setLogitude(cmd[2]);
            la_range = stoi(cmd[3]);
            lo_range = stoi(cmd[4]);
        } else if (cmd.size() == 6) {
            latitude.setlatitude(cmd[2]);
            longitude.setLogitude(cmd[3]);
            la_range = stoi(cmd[4]);
            lo_range = stoi(cmd[5]);
        } else if (cmd.size() == 7) {
            structure = cmd[2];
            latitude.setlatitude(cmd[3]);
            longitude.setLogitude(cmd[4]);
            la_range = stoi(cmd[5]);
            lo_range = stoi(cmd[6]);
        } 
        vector<int> result = coord.search_in_range(latitude.get_DMS_in_sec(),longitude.get_DMS_in_sec(),la_range,lo_range);

        if(!result.empty()){
            if(cmd.size() == 5 || cmd.size() == 6) {
                matching_result+= " The following " + to_string(result.size()) +" feature(s) were found in (" +  latitude.get_string() + " +/- "+ to_string(la_range)+", " + longitude.get_string()+ " +/- "+ to_string(lo_range)+")\n";
            } else if (cmd.size() == 7) {
                matching_result+= "  The following features matching your criteria were found in  (" +  latitude.get_string() + " +/- "+ to_string(la_range)+", " + longitude.get_string()+ " +/- "+ to_string(lo_range)+")\n";
            }
            int count = 0;
            for(int i = 0; i < result.size(); i++) {
                pair<string,GISRecord> single_record = is_contain(result[i]);
                if(single_record.first=="yes"){
                    DMS lati,lot;
                    lati.setlatitude(single_record.second.get_primary_latitude_DMS());
                    lot.setLogitude(single_record.second.get_primary_longitude_DMS());
                    if(cmd.size() == 5) {
                        matching_result +="\n  "+ to_string(result[i]) + ":  \"" + single_record.second.get_feature_name() + "\"  \"" + single_record.second.get_state_alpha() + "\"  \"(" + lati.get_string() + ", "+ lot.get_string()+")";
                    } else if (cmd.size() == 6) {
                        matching_result +="\n  Feature ID   : " + single_record.second.get_feature_id()+"\n";
                        matching_result +="  Feature Name : " + single_record.second.get_feature_name()+"\n";
                        matching_result +="  Feature Cat  : " + single_record.second.get_feature_class()+"\n";
                        matching_result +="  State        : " + single_record.second.get_state_alpha()+"\n";
                        matching_result +="  County       : " + single_record.second.get_country_name()+"\n";
                        matching_result +="  Longitude    : " + lot.get_string()+"\n";
                        matching_result +="  Latitude     : " + lati.get_string()+"\n";
                        matching_result +="  Elev in ft   : " + single_record.second.get_elevation_feet()+"\n";
                        matching_result +="  USGS Quad    : " + single_record.second.get_map_name()+"\n";
                        matching_result +="  Date created : " + single_record.second.get_date_created()+"\n";
                    } else if (cmd.size() == 7) {
                        if(single_record.second.compare_filter(structure)) {
                            matching_result +="\n  "+ to_string(result[i]) + ":  \"" + single_record.second.get_feature_name() + "\"  \"" + single_record.second.get_state_alpha() + "\"  \"(" + lati.get_string() + ", "+ lot.get_string()+")";
                            count++;
                        }
                    }
                    result.erase(result.begin() + i);
                }
            }
            if(!result.empty()){
                vector<GISRecord> remain_record = get_data(result);
                for(int i = 0; i < remain_record.size(); i++) {
                    DMS lati,lot;
                    lati.setlatitude(remain_record[i].get_primary_latitude_DMS());
                    lot.setLogitude(remain_record[i].get_primary_longitude_DMS());
                    if(cmd.size() == 5) {
                        matching_result += "\n  " + to_string(result[i]) + ":  \"" + remain_record[i].get_feature_name() + "\"  \"" + remain_record[i].get_state_alpha() + "\"  \"(" + lati.get_string() + ", "+ lot.get_string()+")";
                    }else if (cmd.size() == 6) {
                        matching_result +="\n  Feature ID   : " + remain_record[i].get_feature_id()+"\n";
                        matching_result +="  Feature Name : " + remain_record[i].get_feature_name()+"\n";
                        matching_result +="  Feature Cat  : " + remain_record[i].get_feature_class()+"\n";
                        matching_result +="  State        : " + remain_record[i].get_state_alpha()+"\n";
                        matching_result +="  County       : " + remain_record[i].get_country_name()+"\n";
                        matching_result +="  Longitude    : " + lot.get_string()+"\n";
                        matching_result +="  Latitude     : " + lati.get_string()+"\n";
                        matching_result +="  Elev in ft   : " + remain_record[i].get_elevation_feet()+"\n";
                        matching_result +="  USGS Quad    : " + remain_record[i].get_map_name()+"\n";
                        matching_result +="  Date created : " + remain_record[i].get_date_created()+"\n";
                    } else if (cmd.size() == 7) {
                        if(remain_record[i].compare_filter(structure)) {
                            matching_result +="\n  "+ to_string(result[i]) + ":  \"" + remain_record[i].get_feature_name() + "\"  \"" + remain_record[i].get_state_alpha() + "\"  \"(" + lati.get_string() + ", "+ lot.get_string()+")";
                            count++;
                        }
                    }
                }
            }
            if(cmd.size() == 7) {
                matching_result+="\n\n  There were "+ to_string(count)+ " features of type " + structure + ".";
            }
        } else {
            matching_result += " Nothing was found in (\"" +latitude.get_string() + " +/- "+ to_string(la_range)+"\", \"" + longitude.get_string()+ " +/- "+ to_string(lo_range)+"\")";
        }
        return matching_result;
    }
    return "cmd error";
}

string BufferPool::str(){
    sort_buffer();
    string result="MRU\n";
    for(int i = 0; i < lru_array.size(); i++) {
        result+= "  "+to_string(file_offset[i])+": ";
        result+= lru_array[i].first.str();
    }
    result+="LRU\n";
    return result;
}
