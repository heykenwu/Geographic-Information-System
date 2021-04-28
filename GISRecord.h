#pragma once
#ifndef GISRECORD_H
#define GISRECORD_H
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
using namespace std;

class GISRecord {
    private:
        string feature_id;
        string feature_name;
        string feature_class;
        string state_alpha;
        string state_numeric;
        string country_name;
        string country_numeric;
        string primary_latitude_DMS;
        string primary_longitude_DMS;
        string primary_latitde_DEC;
        string primary_longitude_DEC;
        string source_latitude_DMS;
        string source_longitude_DMS;
        string source_latitude_DEC;
        string source_longitude_DEC;
        string elevation_meters;
        string elevation_feet;
        string map_name;
        string date_created;
        string date_edited;
    public:
        GISRecord();
        ~GISRecord();
        GISRecord(string feature_id,
        string feature_name,
        string feature_class,
        string state_alpha,
        string state_numeric,
        string country_name,
        string country_numeric,
        string primary_latitude_DMS,
        string primary_longitude_DMS,
        string primary_latitde_DEC,
        string primary_longitude_DEC,
        string source_latitude_DMS,
        string source_longitude_DMS,
        string source_latitude_DEC,
        string source_longitude_DEC,
        string elevation_meters,
        string elevation_feet,
        string map_name,
        string date_created,
        string date_edited);

        string get_feature_id();
        string get_feature_name();
        string get_feature_class();
        string get_state_alpha();
        string get_state_numeric();
        string get_country_name();
        string get_country_numeric();
        string get_primary_latitude_DMS();
        string get_primary_longitude_DMS();
        string get_primary_latitde_DEC();
        string get_primary_longitude_DEC();
        string get_source_latitude_DMS();
        string get_source_longitude_DMS();
        string get_source_latitude_DEC();
        string get_source_longitude_DEC();
        string get_elevation_meters();
        string get_elevation_feet();
        string get_map_name();
        string get_date_created();
        string get_date_edited();

        bool compare_filter(string);

        string double_to_string(double x);
        GISRecord parse_GIS_raw(string raw);

        string str();

};
#endif