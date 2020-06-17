/*
 * Distributed under the OSI-approved Apache License, Version 2.0.  See
 * accompanying file Copyright.txt for details.
 *
 * helloBPWriterXML_nompi.cpp: sequential non-mpi version of helloBPWriterXML
 *
 *  Created on: Feb 16, 2017
 *      Author: William F Godoy godoywf@ornl.gov
 */

#include <ios>       //std::ios_base::failure
#include <iostream>  //std::cout
#include <stdexcept> //std::invalid_argument std::exception
#include <vector>

#include <adios2.h>

#define str_helper(X) #X
#define str(X) str_helper(X)

#ifndef DEFAULT_CONFIG
#define DEFAULT_CONFIG helloBPWriter.xml
#endif
#define DEFAULT_CONFIG_STR str(DEFAULT_CONFIG)

int main(int argc, char *argv[])
{
    std::string configFile;
    if (argc == 1)
    {
        configFile = DEFAULT_CONFIG_STR;
    }
    else if (argc == 2)
    {
        configFile = argv[1];
    }
    else
    {
        std::cerr << "Usage: " << argv[0] << " [/path/to/config.xml]"
                  << std::endl;
        return 1;
    }
    std::cout << "Using config file: " << configFile << std::endl;

    /** Application variable */
    std::vector<float> myFloats = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    const std::size_t Nx = myFloats.size();

    try
    {
        /** ADIOS class factory of IO class objects */
        adios2::ADIOS adios(configFile);

        /*** IO class object: settings and factory of Settings: Variables,
         * Parameters, Transports, and Execution: Engines */
        adios2::IO bpIO = adios.DeclareIO("BPFile_N2N");

        /** global array : name, { shape (total) }, { start (local) }, { count
         * (local) }, all are constant dimensions */
        adios2::Variable<float> &bpFloats = bpIO.DefineVariable<float>(
            "bpFloats", {}, {}, {Nx}, adios2::ConstantDims);

        /** Engine derived class, spawned to start IO operations */
        auto bpWriter = bpIO.Open("myVector.bp", adios2::Mode::Write);

        if (!bpWriter)
        {
            throw std::ios_base::failure(
                "ERROR: bpWriter not created at Open\n");
        }

        /** Write variable for buffering */
        bpWriter->Write<float>(bpFloats, myFloats.data());

        /** Create bp file, engine becomes unreachable after this*/
        bpWriter->Close();
    }
    catch (std::invalid_argument &e)
    {
        std::cout << "Invalid argument exception, STOPPING PROGRAM\n";
        std::cout << e.what() << "\n";
    }
    catch (std::ios_base::failure &e)
    {
        std::cout << "IO System base failure exception, STOPPING PROGRAM\n";
        std::cout << e.what() << "\n";
    }
    catch (std::exception &e)
    {
        std::cout << "Exception, STOPPING PROGRAM from rank\n";
        std::cout << e.what() << "\n";
    }

    return 0;
}
