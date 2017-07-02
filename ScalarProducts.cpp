//    Copyright 2013-2014 University of Pennsylvania
//    Created by Pawel Dlotko
//      Modify by Tran Quoc Hoan 2017/06/29

//    This file is part of Persistence Landscape Toolbox (PLT).
//
//    PLT is free software: you can redistribute it and/or modify
//    it under the terms of the GNU Lesser General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    PLT is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU Lesser General Public License for more details.
//
//    You should have received a copy of the GNU Lesser General Public License
//    along with PLT.  If not, see <http://www.gnu.org/licenses/>.

#include <iostream>
#include <cstdlib>
#include <ctime>
#include "PersistenceBarcode.h"
#include "PersistenceLandscape.h"
#include "FilesReader.h"
#include "PermutationTest.h"

const char* programInfo =
"This is ScalarProduct program which is a part of Peristence Landscape Toolbox by Pawel Dlotko. It takes as input the following parameter: \n\
A file containing a list of files with persistence landscapes (.lan files) or persistence barcodes have to be provided).";

int main(int argc, char *argv[])
{
    clock_t begin = clock();
    configure();

    std::cout << programInfo << std::endl;

    if ( argc < 3 )
    {
        std::cout << std::endl << std::endl;
        std::cout << "Wrong usage of a program. Please call <program name> ";
        std::cout << "<Output calculated file> ";
        std::cout << "<name of the file with names of files with persistence landscapes/barcodes> ";
        std::cout << "The program will now terminate." << std::endl;
        return 1;
    }

    std::vector<std::string> namesOfFiles;
    if (argc == 3){
        char* fileWithInputFiles = argv[2];

        std::cout << "Here are the parameters of the program: " << std::endl;
        std::cout << "Name of the file with barcodes \\ landscapes : " << fileWithInputFiles << std::endl;
        namesOfFiles = readFileNames( fileWithInputFiles );
    }
    else {
        for (int i = 2; i < argc; ++i) {
            std::string tmp_str(argv[i]);
            namesOfFiles.push_back(tmp_str);
        }
    }

    //now generate landscapes based on that files:
    std::cout << "Creating Persistence Landscapes." << std::endl;
    std::vector< PersistenceLandscape > landscapes = createLandscapesFromTheFiles( namesOfFiles );
    std::cout << "Done." << std::endl;

    clock_t beforeComputations = clock();

    //And finally, do the permutation test:
    std::cout << "Computing scalar products. When finished, they will be found in the file 'scalarProducts.txt'" << std::endl;
    ofstream out;
    out.open( argv[1] );

    std::vector< std::vector<double> > scalarProductMatrix;

    for ( size_t i = 0 ; i != landscapes.size() ; ++i )
    {
        for ( size_t j = 0 ; j != landscapes.size() ; ++j )
        {
            out << computeInnerProduct( landscapes[i] , landscapes[j] ) << " ";
        }
        out << endl;
    }


    out.close();
    std::cout << "That is all. Have a good day!" << std::endl;

    clock_t end = clock();

    cerr << "Total execution time : " << double(end - begin) / CLOCKS_PER_SEC << endl;
    cerr << "Computations of bottleneck distance : " << double(end - beforeComputations) / CLOCKS_PER_SEC << endl;

	return 0;
}
