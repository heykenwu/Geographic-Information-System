#include <iostream>
#include "SystemManager.h"
using namespace std;

int main(int arc, char *argv[]) {
    SystemManager cp(arc,argv);
    cp.database_log_creation();
    cp.parse_commands();
    return 0;
}