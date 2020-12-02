#include "WS.hpp"
#include <random>
#include <algorithm>
#include <iterator>
#include <iostream>
#include <functional>
#include <vector>


WS::WS( double arr, double min, double d, double max, double threshold, double pen):
  LAMBDA_a_w(arr), lambda_a_w(min), D_a_w(d) , mu_a_w(max), R_bar_a_w(threshold), nu_a_w(pen){};

WS::WS( double d, double max, double threshold, double pen):
  D_a_w(d) , mu_a_w(max), R_bar_a_w(threshold), nu_a_w(pen){};

double WS::get_LAMBDA_a_w(void)
{
  return LAMBDA_a_w;
}

double WS::get_lambda_a_w(void)
{
  return lambda_a_w;
}

double WS::get_mu_a_w(void)
{
  return mu_a_w;
}

double WS::get_D_a_w(void)
{
  return D_a_w;
}

double WS::get_R_bar_a_w(void)
{
  return R_bar_a_w;
}

double WS::get_nu_a_w(void)
{
  return nu_a_w;
}

bool WS::operator== (const WS & w) const
{
  return (LAMBDA_a_w == w.LAMBDA_a_w && lambda_a_w == w.lambda_a_w && mu_a_w == w.mu_a_w && D_a_w == w.D_a_w &&  R_bar_a_w == w.R_bar_a_w && nu_a_w == w.nu_a_w);
}

void WS::print( void )
{
  std::cout << "LAMBDA_a_w: "<< LAMBDA_a_w<<'\n';
  std::cout << "lambda_a_w: " << lambda_a_w << '\n';
  std::cout << "D_a_w: " << D_a_w <<'\n';
  std::cout << "mu_a_w: " << mu_a_w<<'\n';
  std::cout << "R_bar_a_w: " << R_bar_a_w <<'\n';
  std::cout << "nu_a_w: " << nu_a_w << '\n';
}

void WS::set_LAMBDA_a_w( double value )
{
  LAMBDA_a_w = value;
}

void WS::set_lambda_a_w( double value )
{
  lambda_a_w = value;
}

void WS::set_D_a_w( double value )
{
  D_a_w = value;
}

void WS::set_mu_a_w( double value )
{
  mu_a_w = value;
}

void WS::set_R_bar_a_w( double value )
{
  R_bar_a_w = value;
}

void WS::set_nu_a_w( double value )
{
  nu_a_w = value;
}
