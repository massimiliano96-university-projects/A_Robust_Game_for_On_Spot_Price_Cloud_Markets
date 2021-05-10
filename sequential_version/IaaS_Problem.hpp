#ifndef  _IAAS_PROBLEM_H_
#define  _IAAS_PROBLEM_H_

#include "IaaS.hpp"
#include "SaaS.hpp"
#include <vector>
#include <algorithm>
#include <memory>
#include <iostream>
#include <fstream>


class IaaS_Problem
{
  private:

    IaaS iaas;                                    // IaaS associated to the problem

    std::vector< std::shared_ptr< SaaS > > SaaSs;      // this vector contains all the SaaS

    double omega;                                 // the cost of the energy (omega)

    std::vector< bool > zs;                        // this vector indicates to which SaaSs the IaaS gives the on-spot VMs

    double revenues = 0.0;

    double final_sigma = 0.0;

    double objective_function_value = .0;

    double rho;                                   // this is the cost of on-flat VMs

    double delta;                                 // this is the cost of on-demand VMs

    unsigned N;                                   // this is the total number of VMs available at the IaaS

    std::vector < double > on_spot;             // in this vector we store the number of on_spot VMs for each application of each SaaS

    double total_on_flat = 0;                   // total number of on_flat VMs used by all the SaaSs

    double total_on_demand = 0;                 // total number of on_demand VMs used by all the SaaSs


    void compute_total_on_flat_demand( void ); // this function compute the total number of on_flat and on_demand VMs aked by all the SaaSs

    double compute_total_desired( unsigned );   // I need this function to solve the IaaS problem, it computes the total number
                                                // of desired_on_spot asked by the first t SaaSs
    void reset( void );                         // this function reset the parameters, each time the funtion solve_greedy() is called


  public:

    //constructor
    IaaS_Problem( IaaS , std::vector<std::shared_ptr< SaaS >> , double, double, double, unsigned);
    IaaS_Problem() = default;

    //getters

    double get_sigma( void );

    double get_omega( void );

    unsigned get_N( void ){return N;}

    //setter

    void set_omega( double );

    void set_sigma(double current_sigma){final_sigma = current_sigma;}

    //methods

    void solve_greedy ( void );


    void print ( std::ofstream & );




};

// we define a predicate in order to sort in descending order the SaaSs
struct SortFunctor1
{
    bool operator()(std::shared_ptr< SaaS > object1, std::shared_ptr< SaaS > object2)
    {
      return(object2 -> get_cost_threshold() < object1-> get_cost_threshold() );
    }
};

#endif
