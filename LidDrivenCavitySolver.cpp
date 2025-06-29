#include <iostream>
#include <mpi.h>
#include <cmath>
#include<array>
#include <chrono>
using namespace std;

#include <boost/program_options.hpp>
namespace po = boost::program_options;

#include "LidDrivenCavity.h"

int main(int argc, char **argv)
{
    
    //Time counter, referenced to online resources
    const auto start{std::chrono::steady_clock::now()};



    po::options_description opts(
        "Solver for the 2D lid-driven cavity incompressible flow problem");
    opts.add_options()
        ("Lx",  po::value<double>()->default_value(1.0),
                 "Length of the domain in the x-direction.")
        ("Ly",  po::value<double>()->default_value(1.0),
                 "Length of the domain in the y-direction.")
        ("Nx",  po::value<int>()->default_value(9),
                 "Number of grid points in x-direction.")
        ("Ny",  po::value<int>()->default_value(9),
                 "Number of grid points in y-direction.")
        ("dt",  po::value<double>()->default_value(0.01),
                 "Time step size.")
        ("T",   po::value<double>()->default_value(1.0),
                 "Final time.")
        ("Re",  po::value<double>()->default_value(10),
                 "Reynolds number.")
        ("verbose",    "Be more verbose.")
        ("help",       "Print help message.");

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, opts), vm);
    po::notify(vm);

    if (vm.count("help")) {
        cout << opts << endl;
        return 0;
    }


    LidDrivenCavity* solver = new LidDrivenCavity();
    solver->SetDomainSize(vm["Lx"].as<double>(), vm["Ly"].as<double>());
    solver->SetGridSize(vm["Nx"].as<int>(),vm["Ny"].as<int>());
    solver->SetTimeStep(vm["dt"].as<double>());
    solver->SetFinalTime(vm["T"].as<double>());
    solver->SetReynoldsNumber(vm["Re"].as<double>());

    MPI_Init(&argc, &argv);  

    //
    solver->SetSubCore();
    //

    //Get rank of current porcess
    int rank1=-1;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank1);
    

    solver->PrintConfiguration();


    solver->Initialise();
    

    solver->WriteSolution("ic.txt");


    solver->Integrate();

    solver->WriteSolution("final.txt");


    MPI_Finalize(); 

    const auto end{std::chrono::steady_clock::now()};
    const std::chrono::duration<double> elapsed_seconds{end - start};

    //print out time elapsed
    if (rank1==0){
        cout << "Elaspsed Time = "<< elapsed_seconds.count() << "s\n";
    }

	return 0;
}
