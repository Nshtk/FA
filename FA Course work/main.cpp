#include <iostream>
#include <fstream>
#include <ctime>
#include "client_code.h"

using namespace std;

int main()
{
    ofstream ofp("output_file.txt");
    if(!ofp.is_open())
    {
        err_code=1;
        errCheck_Main();
    }

    srand(static_cast <unsigned>(time(nullptr)));

    if(clientCode(ofp))
        err_code=2;
    ofp.close();

    errCheck_Main();
}
