#include "IaaS.hpp"
#include "IaaS_Problem.hpp"
#include "gurobi_c++.h"
#include <iostream>
#include <fstream>
#include <vector>


IaaS_Problem::IaaS_Problem( IaaS i, std::vector< std::shared_ptr<SaaS >> ss, double energy_cost, double on_flat_cost, double on_demand_cost, unsigned n):
  iaas(i), SaaSs(ss), omega(energy_cost), rho(on_flat_cost), delta(on_demand_cost), N(n)
  {
    zs.resize(SaaSs.size());
  }


void IaaS_Problem::compute_total_on_flat_demand( void ) //this function computes the total number of on_flat and on_demand VMs asked from all the SaaSs
{
  for(unsigned i = 0; i < SaaSs.size(); i++)
  {
    auto app = SaaSs[i] -> get_applications();
    for( unsigned j = 0; j < SaaSs[i] -> get_size(); j++ )
    {
      total_on_flat += SaaSs[i] -> get_on_flat(app[j]);
      total_on_demand += SaaSs[i] -> get_on_demand(app[j]);
    }
  }

  // for debugging
  /*
    std::cout << "total_on_flat= "<< total_on_flat << '\n';
    std::cout << "total_on_demand= "<< total_on_demand << '\n';
  */

}

double IaaS_Problem::compute_total_desired( unsigned t )
{
  double result = .0;
  for ( unsigned i = 0 ; i <= t ; i++ )
  {
    auto apps = SaaSs[i] -> get_applications();
    for( auto & app : apps )
    {
      result += SaaSs[i] -> get_desired_on_spot( app );
    }
  }
  //std::cout << "total_desired =" << result<< '\n'; // for debugging
  return result;
}

void IaaS_Problem::reset( void )
{
  total_on_flat = .0;
  total_on_demand = .0;
  zs.resize(SaaSs.size(),.0);
  std::vector<double> temp;
  on_spot = temp;
}

void IaaS_Problem::solve_greedy ( void )
{

  std::sort(SaaSs.begin(), SaaSs.end(), SortFunctor1() ); // sorting the SaaS in decreasing order with respect to the cost_threshold

  this -> reset();

  this -> compute_total_on_flat_demand();
  double on_spot_available = N - total_on_flat - total_on_demand;


    //std::cout << "on_spot_available: "<< on_spot_available << '\n'; // for debugging


  // final_sigma is already  initialized with 0

  double revenues = .0;
  unsigned t = 0;

  final_sigma = SaaSs[ t ] -> get_cost_threshold();
  //std::cout << " sigma = "<< final_sigma << '\n'; // debugging
  double total_desired_on_spot = compute_total_desired( t );

  double s = std::min( on_spot_available, total_desired_on_spot );

  while( s <  on_spot_available && t < SaaSs.size() )
  {
    double current_revenues = SaaSs[ t ] -> get_cost_threshold() * s;
    if( current_revenues > revenues )
    {
      revenues = current_revenues;
      final_sigma = SaaSs[ t ] -> get_cost_threshold();
      //std::cout << " sigma = "<< final_sigma << '\n'; // debugging
    }
    t = t + 1;
    if (t < SaaSs.size())
    {
      total_desired_on_spot = compute_total_desired( t );/* message */
      s = std::min( on_spot_available, total_desired_on_spot );
    }
  }

  objective_function_value = revenues;

  for( unsigned j = 0; j < SaaSs.size(); j++)
  {
    if( SaaSs[j] -> get_cost_threshold() >= final_sigma )
    {
      zs[ j ] = 1;
      auto apps = SaaSs[j] -> get_applications();
      for(auto & app : apps )
      {
        double s_a = std::min( SaaSs[j] -> get_desired_on_spot(app), on_spot_available );
        SaaSs[j] -> set_given_on_spot( app, s_a);
        on_spot_available -= s_a;
        on_spot.push_back(s_a);
      }
    }
    else
      zs[ j ] = 0;
  }

}

void IaaS_Problem::print ( std::ofstream & ofs )
{
  ofs << "IAAS PROBLEM : "<< '\n';
  ofs << "sigma =  "<< final_sigma << '\n';
  ofs << "objective_function = "<< objective_function_value << '\n';
  ofs << '\n';
}

double IaaS_Problem::get_sigma( void )
{
  return final_sigma;
}

double IaaS_Problem::get_omega( void )
{
  return omega;
}

void IaaS_Problem::set_omega( double value )
{
  omega = value;
}
