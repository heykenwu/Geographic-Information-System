#include "GISRecord.h"

GISRecord::GISRecord(){};
GISRecord::~GISRecord(){};

GISRecord::GISRecord(string f_id,
    string f_name,
    string f_class,
    string s_alpha,
    string s_numeric,
    string c_name,
    string c_numeric,
    string p_latitude_DMS,
    string p_longitude_DMS,
    string p_latitde_DEC,
    string p_longitude_DEC,
    string s_latitude_DMS,
    string s_longitude_DMS,
    string s_latitude_DEC,
    string s_longitude_DEC,
    string eleva_meters,
    string eleva_feet,
    string m_name,
    string d_created,
    string d_edited){
        feature_id = f_id;
        feature_name = f_name;
        feature_class = f_class;
        state_alpha = s_alpha;
        state_numeric = s_numeric;
        country_name = c_name;
        country_numeric = c_numeric;
        primary_latitude_DMS = p_latitude_DMS;
        primary_latitde_DEC = p_latitde_DEC;
        primary_longitude_DMS = p_longitude_DMS;
        primary_longitude_DEC = p_longitude_DEC;
        source_latitude_DMS = s_latitude_DMS;
        source_latitude_DEC = s_latitude_DEC;
        source_longitude_DMS = s_longitude_DMS;
        source_longitude_DEC = s_longitude_DEC;
        elevation_meters = eleva_meters;
        elevation_feet = eleva_feet;
        map_name = m_name;
        date_created = d_created;
        date_edited = d_edited;
    };

string GISRecord::get_feature_id(){
    return feature_id;
}

string GISRecord::get_feature_name(){
    return feature_name;
}

string GISRecord::get_feature_class(){
    return feature_class;
}

string GISRecord::get_state_alpha(){
    return state_alpha;
}

string GISRecord::get_state_numeric(){
    return state_numeric;
}

string GISRecord::get_country_name(){
    return country_name;
}

string GISRecord::get_country_numeric(){
    return country_numeric;
}

string GISRecord::get_primary_latitde_DEC(){
    return primary_latitde_DEC;
}

string GISRecord::get_primary_latitude_DMS(){
    return primary_latitude_DMS;
}

string GISRecord::get_primary_longitude_DEC(){
    return primary_longitude_DEC;
}

string GISRecord::get_primary_longitude_DMS(){
    return primary_longitude_DMS;
}

string GISRecord::get_source_latitude_DEC(){
    return source_latitude_DEC;
}

string GISRecord::get_source_latitude_DMS(){
    return source_latitude_DMS;
}

string GISRecord::get_source_longitude_DEC(){
    return source_longitude_DEC;
}

string GISRecord::get_source_longitude_DMS(){
    return source_longitude_DMS;
} 

string GISRecord::get_elevation_meters(){
    return elevation_meters;
}

string GISRecord::get_elevation_feet(){
    return elevation_feet;
}

string GISRecord::get_map_name(){
    return map_name;
}

string GISRecord::get_date_created(){
    return date_created;
}

string GISRecord::get_date_edited(){
    return date_edited;
}

string GISRecord::str(){
    string result = feature_id + "|";
    result += feature_name + "|";
    result += feature_class + "|";
    result += state_alpha + "|";
    result += state_numeric + "|";
    result += country_name + "|";
    result += country_numeric + "|";
    result += primary_latitude_DMS + "|";
    result += primary_longitude_DMS + "|";
    result += primary_latitde_DEC + "|";
    result += primary_longitude_DEC + "|";
    result += source_latitude_DMS + "|";
    result += source_longitude_DMS + "|";
    result += source_latitude_DEC + "|";
    result += source_longitude_DEC + "|";
    result += elevation_meters + "|";
    result += elevation_feet + "|";
    result += map_name + "|";
    result += date_created + "|";
    result += date_edited + "\n";

    return result;
}

GISRecord GISRecord::parse_GIS_raw(string raw){
    istringstream ss(raw);
    string token; 
    vector<string> GIS;
    while(getline(ss, token, '|')) {
        GIS.push_back(token);
    }
    if(token == ""){
            GIS.push_back(token);
    }
    GISRecord record = GISRecord(GIS[0],GIS[1],GIS[2],GIS[3],GIS[4],GIS[5],GIS[6],GIS[7],GIS[8],GIS[9],GIS[10],GIS[11],GIS[12],GIS[13],GIS[14],GIS[15],GIS[16],GIS[17],GIS[18],GIS[19]);
    return record;
}

string GISRecord::double_to_string(double x){
    stringstream stream;
    stream.precision(7);
    stream << x;
    return stream.str();
}

bool GISRecord::compare_filter(string filter){
    if(filter == "structure"){
        vector<string> classes = {"Airport","Bridge","Building","Church","Dam","Hospital","Levee","Park","Post Office","School","Tower","Tunnel"};
        return (std::find(classes.begin(), classes.end(), feature_class) != classes.end());
    } else if (filter == "water") {
        vector<string> classes = {"Arroyo","Bend","Canal","Channel","Falls","Glacier","Gut","Harbor","Lake","Rapids","Reservoir","Sea","Spring","Stream","Swamp","Well"};
        return (std::find(classes.begin(), classes.end(), feature_class) != classes.end());
    } else if (filter == "pop") {
        vector<string> classes = {"Populated Place"};
        return (std::find(classes.begin(), classes.end(), feature_class) != classes.end());
    }
    return false;
}