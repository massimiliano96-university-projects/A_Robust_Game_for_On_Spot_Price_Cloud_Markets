#include <vector>
#include <iostream>
#include "IaaS.hpp"

const double IaaS::get_sigma( void ) const
{
  return sigma;
}

const std::vector< unsigned > IaaS::get_s_a( void ) const
{
  return s_a;
}

void IaaS::set_sigma( const double d )
{
  sigma = d;
}

void IaaS::set_s_a( unsigned index, unsigned s)
{
  s_a[ index ] = s;
}

void IaaS::print ( void )
{
  std::cout << "the cost of on-spot VMs is: " << sigma <<'\n';
  std::cout<< "the number of on-spot VMs for each application is: "<<'\n';
  for ( unsigned i = 0; i < s_a.size(); i++)
  {
    std::cout<< s_a[i] << " "<<std::endl;
  }
  std::cout << '\n' << '\n';
}
