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

const char* programInfo = "Compute scalar product between barcodes or persistence landscape";

bool is_file_exist(std::string filename){
    std::ifstream infile(&filename[0]);
    return infile.good();
}

int main(int argc, char *argv[])
{
    clock_t begin = clock();
    configure();

    std::cout << programInfo << std::endl;

    if ( argc < 3 )
    {
        std::cout << std::endl << std::endl;
        std::cout << "Usage: <program name> ";
        std::cout << "<Output calculated file> ";
        std::cout << "<name of the file with names of files with persistence landscapes/barcodes> " << std::endl;
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

    // check the input files
    for (int j = 0; j < namesOfFiles.size(); ++j){
        const std::string fname = namesOfFiles[j];
        if (!is_file_exist(fname)){
            std::cout << "File " << fname << " not found" << std::endl;
            std::cout << "Program will be terminated! " << std::endl;
            return 0;
        }
    }
    //now generate landscapes based on that files:
    std::cout << "Number of lands/barcode files " << namesOfFiles.size() << std::endl;
    std::cout << "Creating Persistence Landscapes." << std::endl;
    std::vector< PersistenceLandscape > landscapes = createLandscapesFromTheFiles( namesOfFiles );
    std::cout << "Done." << std::endl;
    if (landscapes.empty()){
        std::cout << "Stop because there's no landscapes" << std::endl;
        return 0;
    }
    clock_t beforeComputations = clock();

    //And finally, do the permutation test:
    std::cout << "Computing scalar products. When finished, they will be found in the file 'scalarProducts.txt'" << std::endl;
    
    //ofstream out;
    //out.open( argv[1] );

    std::vector< std::vector<double> > scalarProductMatrix;

    for ( size_t i = 0 ; i < landscapes.size() ; ++i ){
        for (size_t j = 0; j < i; ++j) std::cout << "  ";
        for ( size_t j = i ; j < landscapes.size() ; ++j ){
            std::cout << computeInnerProduct( landscapes[i] , landscapes[j] ) << " ";
            //out << computeInnerProduct( landscapes[i] , landscapes[j] ) << " ";
        }
        //out << std::endl;
    }

    //out.close();
    std::cout << "That is all. Have a good day!" << std::endl;

    clock_t end = clock();

    cerr << "Total execution time : " << double(end - begin) / CLOCKS_PER_SEC << endl;
    cerr << "Computations of bottleneck distance : " << double(end - beforeComputations) / CLOCKS_PER_SEC << endl;

	return 0;
}
