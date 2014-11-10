/*
 * FileName: main.cpp
 * Description: Main program.
 * Copyright (C) 2014  K M Masum Habib <masum.habib@gmail.com>
 * Created: 02 Nov 2014.
 */

#include "main.h"
#include "config.h"

#include <iostream>
#include <string>

#include <boost/mpi.hpp>
#include <boost/mpi/environment.hpp>
#include <boost/mpi/communicator.hpp>


using namespace std;
using namespace boost::mpi;

/*
 * The main function
 */
int main(int argc, char** argv) {

    const int master_id = 0;
    
    environment env(argc, argv);
    communicator world;
    
    int my_id = world.rank();
    bool master = my_id == master_id;
    
    if (master){
        cout << " Boost.MPI Minimum Project v" << MPIMIN_VERSION << endl;
        cout << " Running on " << world.size() << " CPU(s)" << endl;
    }
    cout << " My ID " << my_id << endl;
    
    return MAIN_SUCCESS;
}

