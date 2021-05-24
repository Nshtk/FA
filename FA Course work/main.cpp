#include <iostream>
#include <fstream>
#include <ctime>
#include "generators.h"
#include "client_code.h"

using namespace std;

int main()
{
    int err_code;                                                   // Замена errno.
    ofstream ofp("output_file.txt");
    if(!ofp.is_open())
    {
        err_code=1;
        errCheck_Main(err_code, 0);
    }

    srand(static_cast <unsigned>(time(nullptr)));

    if(clientCode(ofp, err_code))
        err_code=2;
    ofp.close();

    errCheck_Main(err_code, 0);
}
