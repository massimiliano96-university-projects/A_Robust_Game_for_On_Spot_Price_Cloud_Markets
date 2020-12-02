#include "SaaS.hpp"
#include "application.hpp"
#include <iostream>
#include <vector>
#include  <limits>


const std::vector<application>& SaaS::get_applications (void) const
{
  return applications;
}

std::size_t SaaS::get_size( void )
{
  return applications.size();
}

unsigned SaaS::find_application(const application & a) const //this funtion finds the index of the application a
{
  unsigned count = 0;
  bool found = false;
  while(!found && count != applications.size())
  {
    if(applications[count] == a)
      found = true;
    else
      count++;
  }
  if(found == false)
  {
    std::cerr<<"Application not found"<<std::endl;
    return std::numeric_limits<unsigned>::quiet_NaN();
  }
  else
    return count;
}

//getters
double SaaS::get_on_flat (const application & a) const
{
  unsigned idx =find_application(a);
  return on_flat[idx];
}

double SaaS::get_on_demand (const application & a) const
{
  unsigned idx =this -> find_application(a);
  return on_demand[idx];
}

double SaaS::get_on_spot (const application & a) const
{
  unsigned idx =this -> find_application(a);
  return on_spot[idx];
}

double SaaS::get_desired_on_spot (const application & a) const
{
  unsigned idx =this -> find_application(a);
  return desired_on_spot[idx];
}

double SaaS::get_given_on_spot (const application & a) const
{
  unsigned idx =this -> find_application(a);
  return given_on_spot[idx];
}

double SaaS::get_cost_threshold ( void ) const
{
  return cost_threshold;
}

// we can add an application
void  SaaS::add_application( const application & a)
{
  applications.push_back(a);
  on_flat.push_back(.0);
  on_demand.push_back(.0);
  on_spot.push_back(.0);
  desired_on_spot.push_back(.0);
  given_on_spot.push_back(.0);
}

//setters
void SaaS::set_on_flat ( const application &a, double r)
{
  on_flat[find_application(a)] = r;
}

void SaaS::set_on_demand ( const application &a, double d)
{
  on_demand[find_application(a)] = d;
}

void SaaS::set_on_spot ( const application &a, double s)
{
  on_spot[find_application(a)] = s;
}

void SaaS::set_desired_on_spot ( const application &a, double s)
{
  desired_on_spot[find_application(a)] = s;
}

void SaaS::set_given_on_spot ( const application &a, double s)
{
  given_on_spot[find_application(a)] = s;
}

void SaaS::set_cost_threshold (double cost)
{
  cost_threshold = cost;
}

bool SaaS::operator<( const SaaS& other)
{
  return get_cost_threshold() <= other.get_cost_threshold();
}
