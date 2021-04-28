#include "CoordinateIndex.h"

tree::tree(int la_south,int la_north,int long_west, int long_east) {
        latitude_south = la_south;
        latitude_north = la_north;
        longitude_west = long_west;
        longitude_east = long_east;
        root = NULL;
        top_left = NULL;
        top_right = NULL;
        bot_left = NULL;
        bot_right = NULL;
}

void tree::insert(Record_Location record, vector<int> offset){
    Node *new_node = new Node();
    new_node->setLocation(record.get_lat(),record.get_long());
    for (int i = 0; i < offset.size(); i++) {
        new_node->add_offset(offset[i]);
    }
    if(!inBoundary(record.get_lat(),record.get_long())) {
        return;
    }
    if((root != NULL)&&(root->get_location().same(record))) {
        for (int i = 0; i < offset.size(); i++) {
            root->add_offset(offset[i]);
        }
        return;
    }
    if((top_left == NULL) && (top_right == NULL) && (bot_left == NULL) && (bot_right == NULL) && ((root == NULL))) {
        root = new_node;  
        return;
    } else {
        int top_left_latitude_start = (latitude_south+((abs(latitude_north - latitude_south))/2));
        int top_left_latitude_end = latitude_north;      
        int top_left_longitude_start = longitude_west;
        int top_left_longitude_end = (longitude_west+abs(longitude_east-longitude_west)/2);
        
        int top_right_latitude_start = (latitude_south+(abs(latitude_north - latitude_south)/2));
        int top_right_latitude_end = latitude_north;
        int top_right_longitude_start = (longitude_west+abs(longitude_east-longitude_west)/2);
        int top_right_longitude_end = longitude_east;

        int bot_left_latitude_start = latitude_south;
        int bot_left_latitude_end = (latitude_south+(abs(latitude_north - latitude_south)/2));
        int bot_left_longitude_start = longitude_west;
        int bot_left_longitude_end = (longitude_west+abs(longitude_east-longitude_west)/2); 

        int bot_right_latitude_start = latitude_south;
        int bot_right_latitude_end = (latitude_south+(abs(latitude_north - latitude_south)/2));
        int bot_right_longitude_start = (longitude_west+abs(longitude_east-longitude_west)/2);
        int bot_right_longitude_end = longitude_east;    

        if(root != NULL) {
            if((root->get_location().get_lat() >= top_left_latitude_start) && (root->get_location().get_lat() <= top_left_latitude_end) && ((root->get_location().get_long() >= top_left_longitude_start) && (root->get_location().get_long() <= top_left_longitude_end))) {
                Node *tmp = root;
                root = NULL;
                if(top_left != NULL) {
                    top_left->insert(tmp->get_location(),tmp->get_offsets());
                } else {
                    top_left = new tree(top_left_latitude_start,top_left_latitude_end,top_left_longitude_start,top_left_longitude_end);
                    top_left->insert(tmp->get_location(),tmp->get_offsets());
                }
            } else if ((root->get_location().get_lat() >= top_right_latitude_start) && (root->get_location().get_lat() <= top_right_latitude_end) && (root->get_location().get_long() >= top_right_longitude_start) && (root->get_location().get_long() <= top_right_longitude_end)) {
                Node *tmp = root;
                root = NULL;
                if(top_right != NULL) {
                    top_right->insert(tmp->get_location(),tmp->get_offsets());
                } else {
                    top_right = new tree(top_right_latitude_start,top_right_latitude_end,top_right_longitude_start,top_right_longitude_end);
                    top_right->insert(tmp->get_location(),tmp->get_offsets());
                }
            } else if ((root->get_location().get_lat() >= bot_left_latitude_start) && (root->get_location().get_lat() <= bot_left_latitude_end) && (root->get_location().get_long() >= bot_left_longitude_start) && (root->get_location().get_long() <= bot_left_longitude_end)) {
                Node *tmp = root;
                root = NULL;
                if(bot_left != NULL) {
                    bot_left->insert(tmp->get_location(),tmp->get_offsets());
                } else {
                    bot_left = new tree(bot_left_latitude_start,bot_left_latitude_end,bot_left_longitude_start,bot_left_longitude_end);
                    bot_left->insert(tmp->get_location(),tmp->get_offsets());
                }
            } else if ((root->get_location().get_lat() >= bot_right_latitude_start) && (root->get_location().get_lat() <= bot_right_latitude_end) && (root->get_location().get_long() >= bot_right_longitude_start) && (root->get_location().get_long() <= bot_right_longitude_end)) {
                Node *tmp = root;
                root = NULL;
                if(bot_right != NULL) {
                    bot_right->insert(tmp->get_location(),tmp->get_offsets());
                } else {
                    bot_right = new tree(bot_right_latitude_start,bot_right_latitude_end,bot_right_longitude_start,bot_right_longitude_end);
                    bot_right->insert(tmp->get_location(),tmp->get_offsets());
                }               
            }
        }
        if((new_node->get_location().get_lat() >= top_left_latitude_start) && (new_node->get_location().get_lat() <= top_left_latitude_end) && (new_node->get_location().get_long() >= top_left_longitude_start) && (new_node->get_location().get_long() <= top_left_longitude_end)) {
            if(top_left != NULL) {
                top_left->insert(record,offset);
            } else {
                top_left = new tree(top_left_latitude_start,top_left_latitude_end,top_left_longitude_start,top_left_longitude_end);
                top_left->insert(record,offset);
            }
            return;  
        } else if ((new_node->get_location().get_lat() >= top_right_latitude_start) && (new_node->get_location().get_lat() <= top_right_latitude_end) && (new_node->get_location().get_long() >= top_right_longitude_start) && (new_node->get_location().get_long() <= top_right_longitude_end)) {
            if(top_right != NULL) {
                top_right->insert(record,offset);
            } else {
                top_right = new tree(top_right_latitude_start,top_right_latitude_end,top_right_longitude_start,top_right_longitude_end);
                top_right->insert(record,offset);
            }
            return;
        } else if ((new_node->get_location().get_lat() >= bot_left_latitude_start) && (new_node->get_location().get_lat() <= bot_left_latitude_end) && (new_node->get_location().get_long() >= bot_left_longitude_start) && (new_node->get_location().get_long() <= bot_left_longitude_end)) {
            if(bot_left != NULL) {
                bot_left->insert(record,offset);
            } else {
                bot_left = new tree(bot_left_latitude_start,bot_left_latitude_end,bot_left_longitude_start,bot_left_longitude_end);
                bot_left->insert(record,offset);
            }
            return;
        } else if ((new_node->get_location().get_lat() >= bot_right_latitude_start) && (new_node->get_location().get_lat() <= bot_right_latitude_end) && (new_node->get_location().get_long() >= bot_right_longitude_start) && (new_node->get_location().get_long() <= bot_right_longitude_end)) {
            if(bot_right != NULL) {
                bot_right->insert(record,offset);
            } else {
                bot_right = new tree(bot_right_latitude_start,bot_right_latitude_end,bot_right_longitude_start,bot_right_longitude_end);
                bot_right->insert(record,offset);
            }
            return;   
        }
    }
}

vector<int> tree::search_single(int l, int lo){
    vector<int> tmp;
    if((top_left == NULL) && (top_right == NULL) && (bot_left == NULL) && (bot_right == NULL)){
        if (root != NULL) {
            if((root->get_location().get_lat() == l) && (root->get_location().get_long() == lo)) {
                tmp = root->get_offsets();
                return tmp;
            } else {
                return tmp;
            }
        }
        return tmp;
    } else {
        int top_left_latitude_start = (latitude_south+((abs(latitude_north - latitude_south))/2));
        int top_left_latitude_end = latitude_north;      
        int top_left_longitude_start = longitude_west;
        int top_left_longitude_end = (longitude_west+abs(longitude_east-longitude_west)/2);

        int top_right_latitude_start = (latitude_south+(abs(latitude_north - latitude_south)/2));
        int top_right_latitude_end = latitude_north;
        int top_right_longitude_start = (longitude_west+abs(longitude_east-longitude_west)/2);
        int top_right_longitude_end = longitude_east;

        int bot_left_latitude_start = latitude_south;
        int bot_left_latitude_end = (latitude_south+(abs(latitude_north - latitude_south)/2));
        int bot_left_longitude_start = longitude_west;
        int bot_left_longitude_end = (longitude_west+abs(longitude_east-longitude_west)/2); 

        int bot_right_latitude_start = latitude_south;
        int bot_right_latitude_end = (latitude_south+(abs(latitude_north - latitude_south)/2));
        int bot_right_longitude_start = (longitude_west+abs(longitude_east-longitude_west)/2);
        int bot_right_longitude_end = longitude_east;  

        if((l >= top_left_latitude_start) && (l <= top_left_latitude_end) && (lo >= top_left_longitude_start) && (lo <= top_left_longitude_end)) {
            if(top_left != NULL) {
                return (top_left->search_single(l,lo));
            } else {
                return tmp;
            } 
        } else if ((l >= top_right_latitude_start) && (l <= top_right_latitude_end) && (lo >= top_right_longitude_start) && (lo <= top_right_longitude_end)) {
            if (top_right != NULL) {
                return (top_right->search_single(l,lo));
            } else {
                return tmp;
            }
        } else if ((l >= bot_left_latitude_start) && (l <= bot_left_latitude_end) && (lo >= bot_left_longitude_start) && (lo <= bot_left_longitude_end)) {
            if (bot_left != NULL) {
                return (bot_left->search_single(l,lo));
            } else {
                return tmp;
            }
        } else if ((l >= bot_right_latitude_start) && (l <= bot_right_latitude_end) && (lo >= bot_right_longitude_start) && (lo <= bot_right_longitude_end)) {
            if(bot_right != NULL) {
                return (bot_right->search_single(l,lo));
            } else {
                return tmp;
            }
        }
    }
    return tmp; 
}

tree tree::search_by_range_Util(int la, int lo, int la_range, int lo_range){
    int la_start = la - la_range;
    int la_end = la + la_range;
    int lo_start = lo - lo_range;
    int lo_end = lo + lo_range;

    int top_left_latitude_start = (latitude_south+((abs(latitude_north - latitude_south))/2));
    int top_left_latitude_end = latitude_north;      
    int top_left_longitude_start = longitude_west;
    int top_left_longitude_end = (longitude_west+abs(longitude_east-longitude_west)/2);

    int top_right_latitude_start = (latitude_south+(abs(latitude_north - latitude_south)/2));
    int top_right_latitude_end = latitude_north;
    int top_right_longitude_start = (longitude_west+abs(longitude_east-longitude_west)/2);
    int top_right_longitude_end = longitude_east;

    int bot_left_latitude_start = latitude_south;
    int bot_left_latitude_end = (latitude_south+(abs(latitude_north - latitude_south)/2));
    int bot_left_longitude_start = longitude_west;
    int bot_left_longitude_end = (longitude_west+abs(longitude_east-longitude_west)/2); 

    int bot_right_latitude_start = latitude_south;
    int bot_right_latitude_end = (latitude_south+(abs(latitude_north - latitude_south)/2));
    int bot_right_longitude_start = (longitude_west+abs(longitude_east-longitude_west)/2);
    int bot_right_longitude_end = longitude_east;  

    if((la_start >= top_left_latitude_start) && (la_end <= top_left_latitude_end) && (lo_start >= top_left_longitude_start) && (lo_end <= top_left_longitude_end)) {
        if(top_left != NULL) {
            return (top_left->search_by_range_Util(la, lo, la_range, lo_range));
        } else {
            return *this;
        } 
    } else if ((la_start >= top_right_latitude_start) && (la_end <= top_right_latitude_end) && (lo_start >= top_right_longitude_start) && (lo_end <= top_right_longitude_end)) {
        if (top_right != NULL) {
            return (top_right->search_by_range_Util(la, lo, la_range, lo_range));
        } else {
            return *this;
        }
    } else if ((la_start >= bot_left_latitude_start) && (la_end <= bot_left_latitude_end) && (lo_start >= bot_left_longitude_start) && (lo_end <= bot_left_longitude_end)) {
        if (bot_left != NULL) {
            return (bot_left->search_by_range_Util(la, lo, la_range, lo_range));
        } else {
            return *this;
        }
    } else if ((la_start >= bot_right_latitude_start) && (la_end <= bot_right_latitude_end) && (lo_start >= bot_right_longitude_start) && (lo_end <= bot_right_longitude_end)) {
        if(bot_right != NULL) {
            return (bot_right->search_by_range_Util(la, lo, la_range, lo_range));
        } else {
            return *this;
        }
    }
    return *this;
}

void tree::search_by_range(int la, int lo, int la_range, int lo_range, tree* subtree, vector<int> &vec){
    int la_start = la - la_range;
    int la_end = la + la_range;
    int lo_start = lo - lo_range;
    int lo_end = lo + lo_range;
    if((subtree->top_left == NULL) && (subtree->top_right == NULL) && (subtree->bot_left == NULL) && (subtree->bot_right == NULL)){
        if (subtree->root != NULL) {
            int lat = subtree->root->get_location().get_lat();
            int longi = subtree->root->get_location().get_long();
            if((lat >= la_start) && ((lat <= la_end)) && (longi >= lo_start) && (longi <= lo_end)) {
                vector<int> tmp = subtree->root->get_offsets();
                vec.insert(std::end(vec), std::begin(tmp), std::end(tmp));
            }
        }
    } else {
        if (subtree->top_left  != NULL){
            search_by_range(la, lo, la_range, lo_range, subtree->top_left,vec);
        } 
        if (subtree->top_right != NULL) {
            search_by_range(la, lo, la_range, lo_range, subtree->top_right,vec);
        } 
        if (subtree->bot_left  != NULL) {
            search_by_range(la, lo, la_range, lo_range, subtree->bot_left,vec);
        } 
        if (subtree->bot_right != NULL) {
            search_by_range(la, lo, la_range, lo_range, subtree->bot_right,vec);
        }
    }
        
}


bool tree::inBoundary(int la,int lo){
    if ((la < latitude_south) || (la > latitude_north)) {
        return false;
    } else if ((lo < longitude_west) || (lo > longitude_east)){
        return false;
    }
    return true;
}

string tree::print_Util(string prefix, tree* root_,int position){
    string posit = "";
    string result=""; 
    switch(position) {
        case 1:
        posit = "NW";
        break;
        case 2:
        posit = "NE";
        break;
        case 3:
        posit = "SW";
        break;
        case 4:
        posit = "SE";
        break;
        default:
        posit="root";            
    } 
    if(root_ != NULL) {
        if(root_->root == NULL) { 
            result+= prefix + "|----@(" + posit +")" + "\n";
            result+= print_Util((prefix+ "|    "),root_->top_left,1);
            result+= print_Util((prefix+ "|    "),root_->top_right,2);
            result+= print_Util((prefix+ "|    "),root_->bot_left,3);
            result+= print_Util((prefix+ "|    "),root_->bot_right,4);
        } else { 
            int size = root_->root->get_offsets().size();
            string tmp = "[("+ to_string(root_->root->get_location().get_lat()) +","+to_string(root_->root->get_location().get_long())+"), [";
            tmp+= to_string(root_->root->get_offsets().at(0));
            if(size > 1){
                for (int i=1; i < size; i++){
                    tmp+= ","+to_string(root_->root->get_offsets().at(i));
                }
            }
            tmp += "]]";
            result+= prefix +"|----"+ tmp +"\n";
        }
    } else { 
        result+= prefix +"|----" + posit +"(NULL)" + "\n";         
    }
    return result;
}
void tree::get_size(tree* subtree,int& size){
    if((subtree->top_left == NULL) && (subtree->top_right == NULL) && (subtree->bot_left == NULL) && (subtree->bot_right == NULL)){
        if (subtree->root != NULL) {
            size++;
        }
    } else {
        if (subtree->top_left  != NULL){
            get_size(subtree->top_left,size);
        } 
        if (subtree->top_right != NULL) {
            get_size(subtree->top_right,size);
        } 
        if (subtree->bot_left  != NULL) {
            get_size(subtree->bot_left,size);
        } 
        if (subtree->bot_right != NULL) {
            get_size(subtree->bot_right,size);
        }
    }
}
string tree::print(tree* root_){
    return print_Util("",root_,0);
}

CoordinateIndex::CoordinateIndex(int ls, int ln, int lw, int le){
    quadtree = tree(ls,ln,lw,le);
}

void CoordinateIndex::insert(int l, int lo, vector<int> offset){
    Record_Location tmp = Record_Location(l,lo);
    quadtree.insert(tmp,offset);
}

vector<int> CoordinateIndex::search_single(int latitude, int longitude){
    vector<int> result = quadtree.search_single(latitude,longitude);
    return result;
}

string CoordinateIndex::print(){
    tree *qua_pointer = &quadtree;
    return quadtree.print(qua_pointer);
}

vector<int> CoordinateIndex::search_in_range(int la, int lo, int la_range, int lo_range){
    vector<int> vec;
    tree subtree = quadtree.search_by_range_Util(la,lo,la_range,lo_range);
    quadtree.search_by_range(la,lo,la_range,lo_range,&subtree,vec);
    return vec;
}

int CoordinateIndex::get_size(){
    int size = 0;
    quadtree.get_size(&quadtree,size);
    return size;
}





