#include "SystemManager.h"

SystemManager::SystemManager(){}

SystemManager::~SystemManager(){logger.close();}

SystemManager::SystemManager(int arc, char *argv[]){
    if(arc != 4) {
        cout << "./GIS <database-file> <script_file.txt> <log_file.txt>" << endl;
        exit(1);
    } else {
        data_file = argv[1];
        script_file = argv[2];
        log_file = argv[3];
    }
}

Command SystemManager::convert_command_type(string x) {
    Command cmd;
    if(x == "world") {
        cmd = WORLD;
    } else if (x == "import"){
        cmd = IMPORT;
    } else if (x == "debug") {
        cmd = DEBUG;
    } else if (x == "what_is_at") {
        cmd = WHAT_IS_AT;
    } else if (x == "what_is") {
        cmd = WHAT_IS;
    } else if (x == "what_is_in") {
        cmd = WHAT_IS_IN;
    } else if (x == "quit") {
        cmd = QUIT;
    }
    return cmd;
}

void SystemManager::database_log_creation(){
    ofstream data_fstream(data_file,  ofstream::trunc | ofstream::out );
    data_fstream << "FEATURE_ID|FEATURE_NAME|FEATURE_CLASS|STATE_ALPHA|STATE_NUMERIC|COUNTY_NAME|COUNTY_NUMERIC|PRIMARY_LAT_DMS|PRIM_LONG_DMS|PRIM_LAT_DEC|PRIM_LONG_DEC|SOURCE_LAT_DMS|SOURCE_LONG_DMS|SOURCE_LAT_DEC|SOURCE_LONG_DEC|ELEV_IN_M|ELEV_IN_FT|MAP_NAME|DATE_CREATED|DATE_EDITED" << endl;
    ofstream log_fstream(log_file, ofstream::trunc | ofstream::out );
    data_fstream.close();
    log_fstream.close();
}

void SystemManager::parse_commands() {
    CommandProcessor cmd_pro = CommandProcessor();
    command_list = cmd_pro.load_file(script_file);
    Logger logger(log_file);
    NameIndex nameIndex;
    CoordinateIndex coordinateIndex;
    vector<DMS> world;
    BufferPool bf_pool = BufferPool(data_file);
    int index = 1;
    string name="";
    logger.write_header(data_file,script_file,log_file);
    for(int i = 0; i < command_list.size(); i++) {
        if(command_list[i].find(";") != string::npos) {
            logger.write_comment(command_list[i]);
        } else {
            logger.write_command(index,command_list[i]);
            logger.write_comment("");
            index++;
            vector<string> cmd_array = seperate_command(command_list[i]);
            Command cmd = convert_command_type(cmd_array[0]);
            switch(cmd) {
                case WORLD:
                    world = cmd_pro.world(cmd_array);
                    logger.write_world(world[0].get_DMS_in_sec(),world[1].get_DMS_in_sec(),world[2].get_DMS_in_sec(),world[3].get_DMS_in_sec());
                    break;
                case IMPORT:
                    cmd_pro.import_database(cmd_array[1],data_file);
                    nameIndex = cmd_pro.name_hashtable_import(data_file,world);
                    coordinateIndex = cmd_pro.coord_import(data_file,world[2].get_DMS_in_sec(),world[3].get_DMS_in_sec(),world[0].get_DMS_in_sec(),world[1].get_DMS_in_sec());
                    logger.write_comment(import_str(nameIndex.size_by_name(),nameIndex.longest_probe(),coordinateIndex.get_size(),nameIndex.name_length()));
                    break;
                case DEBUG:
                    if(cmd_array[1] == "quad") {
                        logger.write_comment(coordinateIndex.print());
                    } else if (cmd_array[1] == "hash") {
                        logger.write_comment(nameIndex.str());
                    } else if (cmd_array[1] == "pool") {
                        logger.write_comment(cmd_pro.debug_pool(bf_pool));
                    } else if (cmd_array[1] == "world") {
                        logger.write_comment("[OPTIONAL]");
                    }
                    break;
                case WHAT_IS_AT:
                    logger.write_comment(cmd_pro.whatIsAt(cmd_array,bf_pool,coordinateIndex));
                    break;
                case WHAT_IS:
                    name = cmd_array[1];
                    for(int i = 2; i < (cmd_array.size() - 1); i++) {
                        name +=  " " + cmd_array[i];
                    }
                    name+=cmd_array.back();
                    logger.write_comment(bf_pool.search_by_name_state(name,nameIndex));
                    break;
                case WHAT_IS_IN:
                    logger.write_comment(cmd_pro.whatIsIn(cmd_array,bf_pool,coordinateIndex));
                    break;
                case QUIT:
                    logger.write_comment("Terminating execution of commands.");
                    logger.write_endline();
                    logger.write_time(2);
                    exit(2);
                    break;
            }
            logger.write_endline();
        }
    }
    logger.write_time(2);
    logger.close();
}

vector<string> SystemManager::seperate_command(string cmd) {
    vector<string> result;
    istringstream iss(cmd);
    string token;
    while(iss >> token) {
        result.push_back(token);
    }
    return result;
}

string SystemManager::import_str(int name_size,int max, int lo, int average_name) {
    string result = "";
    result+= "Imported Features by name: " + to_string(name_size) +"\n";
    result+= "Longest probe sequence:    "+to_string(max)+"\n";
    result+= "Imported Locations:        " + to_string(lo)+"\n";
    result+= "Average name length:       "+ to_string(average_name);

    return result;
}
