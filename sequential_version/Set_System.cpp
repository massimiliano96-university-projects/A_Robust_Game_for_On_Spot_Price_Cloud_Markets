#include "Set_System.hpp"
#include "SaaS.hpp"
#include "application.hpp"
#include "WS.hpp"
#include "SaaS_Problem.hpp"
#include "IaaS.hpp"
#include "IaaS_Problem.hpp"
#include <random>
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <memory>
#include <sstream>
#include <fstream>
#include <ios>
#include <string>

/*
we pass to the constructor the seed and the name of the config_files, from it we set the number
of SaaSs,the number of the apps and the WSs, the we use the set function, in order to create
the object
*/
Set_System::Set_System(unsigned s, const std::string& config_file_name): seed(s)
{
  std::fstream config_file;
  config_file.open( config_file_name, std::ios::in );

  std::string colnames;
  std::getline( config_file, colnames );

  std::string line;
  std::getline(config_file, line );

  std::stringstream ss(line);
  std::vector<std::string> parameters;
  std::string cell;
  while( std::getline( ss , cell, ',') )
  {
    parameters.push_back( cell );
  }

  Number_of_SaaSs = std::stoi( parameters[0] );
  Number_of_apps = std::stoi( parameters[1] );
  Number_of_WSs = std::stoi( parameters[2] );
  time = std::stod( parameters[3] );
}


WS Set_System::set_WS( unsigned j)
{
  unsigned new_seed = seed + j;
  std::mt19937 gen(new_seed);

  // we generate all the distributions
  //std::uniform_int_distribution<> LAMBDA_a_w_distro(400,800);
  std::uniform_int_distribution<> mu_a_w_distro(200,400);
  std::uniform_real_distribution<> D_a_w_distro(0.001, 0.050);


  // now we determine the values from the distributions
  //int LAMBDA_a_w =LAMBDA_a_w_distro(gen);                             // overall prediction of the arrival rate for WS w of application a
  double mu_a_w = mu_a_w_distro(gen);                                   // maximum service rate for executng WS class w of application a
  double D_a_w = D_a_w_distro(gen);                                     // queueing  delay for executing WS  class w of application a
  double nu_a_w = 0.00000045;                                           // penalty for rejecting WS w of the application a
  double R_bar_a_w = (1.5)*( 1/mu_a_w + D_a_w );                        // WS w average response time threshold

  // now we create the WS
  WS web_service(D_a_w, mu_a_w, R_bar_a_w, nu_a_w);
  return web_service;
}



application Set_System::set_application ( unsigned j)
{
  unsigned new_seed = seed + j;
  std::mt19937 gen(new_seed);

  // just use the set_WS function in order to create Number_of_WSs we_services
  std::vector<WS> web_services(Number_of_WSs);

  std::uniform_real_distribution<> p_distro(0.05,0.95); //random distribution of the weights for LAMBDA_a_w
  std::uniform_int_distribution<> K_distro(1,2);  // random distribution of K (number of saturated machines)

  int K = K_distro(gen);
  double p_sum = 0;

  std::vector<double> p_vect(Number_of_WSs);
  for(unsigned k=0; k< Number_of_WSs; k++)
  {
    new_seed += k;
    std::mt19937 gen(new_seed);
    double p = p_distro(gen);
    p_vect[k] = p;  // we the store the weight
    p_sum += p;  // we sum the weights
  }

  //int count_1 = 0;
  for(double &p : p_vect){
        p = p/p_sum; // the weights have to sum to 1
        //std::cout << "p= "<<p_vect[count_1] << '\n';
        //count_1++;
  }


  double sum_p_mu = 0;  // initialize to zero the sum of p_a_w * mu_a_w

  for(unsigned k=0; k< Number_of_WSs; k++)
  {
    web_services[k] = set_WS( k + j );
    sum_p_mu += p_vect[k]/web_services[k].get_mu_a_w();
  }
  //std::cout << "sum_p_mu= "<<sum_p_mu << '\n';

  double LAMBDA_a = K/sum_p_mu;     // we compute the Lambda_a
  //std::cout << "LAMBDA_a "<< LAMBDA_a << '\n';

  unsigned count = 0;
  for(auto &ws : web_services)
  {
    double LAMBDA_a_w = LAMBDA_a * p_vect[count]; // finally we have LAMBDA_a_w
    //std::cout << "LAMBDA_a_w= "<<LAMBDA_a_w << '\n';
    ws.set_LAMBDA_a_w(time*LAMBDA_a_w);
    double lambda_a_w = 0.8 *time* LAMBDA_a_w;    // minimum arrival rate to be guaranteed for WS w of application a
    //double lambda_a_w = 100;
    //std::cout << "lambda_a_w"<<lambda_a_w << '\n';
    ws.set_lambda_a_w(lambda_a_w);
    count++;
  }

  application app(web_services);
  return app;
}

std::shared_ptr<SaaS> Set_System::set_SaaS( unsigned i)
{
  // we create a vector of applications exploiting the function set_application, and we use it to initialize a SaaS
  std::vector<application> apps(Number_of_apps);
  for(unsigned j =0; j< Number_of_apps; j++)
  {
    apps[j] = set_application( i + j );  //we create an app
  }
  SaaS s(apps); // we create the SaaS giving to the constructor the vector of the applications
  return std::make_shared<SaaS>(s);
}

std::vector<SaaS_Problem> Set_System::set_SaaS_problem ( void )
{
  unsigned N = 1;
  unsigned T = 3600;
  std::vector<SaaS_Problem> system(Number_of_SaaSs);    // we create a vector of SaaS_Problem

  for( unsigned i=0; i < Number_of_SaaSs; i++)
  {
    // create a SaaS exploiting the funtions set_SaaS
    std::shared_ptr<SaaS> s = set_SaaS( i );
    double R_j =  N / (3 * Number_of_SaaSs);

    std::uniform_real_distribution<> qj_distro ( 0.25, 0.9 );
    //std::uniform_real_distribution<> rho_distro( 0.01, 0.3)

    unsigned new_seed = seed + i;
    std::mt19937 gen(new_seed);

    double qj = qj_distro(gen);

    // create a SaaS_Problem
    SaaS_Problem element (s, qj ,R_j);

    std::uniform_real_distribution<> rho_distro(0.015, 4.00);
    double rho = rho_distro(gen);
    double delta = 4*rho;

    element.set_rho(rho);
    element.set_delta(delta);
    element.set_T(T);
    element.set_N(N);
    system[i] = element;
  }
  return system;
}


std::pair< IaaS_Problem, std::vector<SaaS_Problem> > Set_System::set(void)
{
  IaaS iaas; //we create the IaaS using the default constructor

  std::vector<SaaS_Problem> system_of_SaaS = set_SaaS_problem(); // we create all the SaaS Problems
  std::sort(system_of_SaaS.begin(), system_of_SaaS.end(), SortFunctor()); // and we sort them wrt the cost_threshold

  double rho = system_of_SaaS[0].get_rho();
  double delta = system_of_SaaS[0].get_delta();
  unsigned N = system_of_SaaS[0].get_N();

  std::mt19937 gen(seed);
  std::uniform_real_distribution<> mu_dev_distro(0.024, 0.42);   //random distribution for the deviation of mu
  std::uniform_real_distribution<> D_dev_distro(0.024, 0.42);    //random distribution for the deviation of D
  double mu_dev = mu_dev_distro(gen);
  double D_dev = D_dev_distro(gen);

  std::vector<std::shared_ptr<SaaS>> SaaSs;
  for(unsigned i = 0; i < system_of_SaaS.size(); i++)  // we set the deviations in each SaaS_Problems instance
  {
    SaaS_Problem& current_SaaS_Problem = system_of_SaaS[i];
    current_SaaS_Problem.set_mu_dev(mu_dev);
    current_SaaS_Problem.set_D_dev(D_dev);
    SaaSs.push_back(current_SaaS_Problem.get_SaaS()); //store the SaaS_Problem in a vector, it is needed for the IaaS_Problem constructor
  }

  // half of the minimum of the cost_threshold
  double omega = 0.5 * ( (*(SaaSs.end()-1)) -> get_cost_threshold() );  //set omega

  IaaS_Problem system_of_IaaS( iaas, SaaSs, omega, rho, delta, N );  // cretae the IaaS_Problem

  std::pair< IaaS_Problem, std::vector<SaaS_Problem> > result = std::make_pair(system_of_IaaS, system_of_SaaS);

  return result;
}


//this function uses the three files passed through the command line to set the entire Problem, this is used after a first initializazion of the parameters during which we set all the create_files
std::pair< IaaS_Problem, std::vector<SaaS_Problem> > Set_System::set_from_files(const std::string& global_file_name, const std::string& SaaSs_file_name, const std::string& WSs_file_name)
{
  // here we'll store the SaaS_Problem to return
  std::vector< SaaS_Problem > SaaS_Problems;

  // this vector is needed for the constructor of the IaaS_Problem
  std::vector< std::shared_ptr<SaaS> > SaaSs;

  // initializethe IaaS
  IaaS iaas;

  // get the parameters from the global_file
  std::fstream global_file;
  global_file.open( global_file_name, std::ios::in );

  //discard the first line
  std::string colnames;
  std::getline( global_file, colnames );

  std::string line;
  std::getline( global_file, line );

  std::stringstream s(line);
  std::string cell;
  std::vector<std::string> v;

  while( std::getline( s, cell, ',') )
  {
    v.push_back( cell );
  }

  unsigned T = std::stoul( v[0] );
  unsigned N = std::stoul( v[1] );
  int gamma = std::stoi( v[2] );
  double rho = std::stod( v[3] );
  double delta = std::stod( v[4] );
  double mu_dev = std::stod( v[5] );
  double D_dev = std::stod( v[6] );
  double omega = std::stod( v[7] );

  // open the SaaSs_file
  std::fstream SaaSs_file;
  SaaSs_file.open( SaaSs_file_name, std::ios::in );

  //discard the first line
  std::getline( SaaSs_file, colnames );

  //open the WSs_file
  std::fstream WSs_file;
  WSs_file.open( WSs_file_name, std::ios::in );

  // discard the first line
  std::getline( WSs_file, colnames );


  // now we have to create all the WSs, then all the applications and finally all the SaaSs from the SaaSs_file and the WSs_file
  for( unsigned i = 0; i < Number_of_SaaSs; i ++)
  {
    //initialize the SaaS
    SaaS current_SaaS;
    for( unsigned j = 0;j < Number_of_apps; j ++)
    {
      //initialize the application
      application current_application;

      std::string line;
      unsigned WS_index = 0;
      while( WS_index < Number_of_WSs )
      {
        std::getline( WSs_file, line );
        std::stringstream s(line);
        std::string cell;
        std::vector<std::string> row;
        while( std::getline( s, cell, ',') )
        {
          row.push_back( cell );
        }
        double LAMBDA_a_w = std::stod( row[0] );
        double lambda_a_w = std::stod( row[1] );
        double D_a_w = std::stod( row[2] );
        double mu_a_w = std::stod( row[3] );
        double R_bar_a_w = std::stod( row[4] );
        double nu_a_w = std::stod( row[5] );

        WS current_WS(LAMBDA_a_w, lambda_a_w, D_a_w, mu_a_w, R_bar_a_w, nu_a_w);

        current_application.add_WS(current_WS);

        WS_index++;
      }
      current_SaaS.add_application(current_application);
    }

    std::string line;
    std::getline( SaaSs_file, line );

    std::stringstream s(line);
    std::string cell;
    std::vector<std::string> row;

    while( std::getline( s, cell, ',') )
    {
      row.push_back( cell );
    }
    double q_j = std::stod( row[0] );
    double eta_j = std::stod( row[1] );
    unsigned R_j = std::stoul( row[2] );

    auto current_SaaS_ptr = std::make_shared<SaaS>(current_SaaS);

    SaaS_Problem current_saas_problem( current_SaaS_ptr , T, N, gamma, rho, delta, q_j, eta_j, R_j, mu_dev, D_dev );

    SaaS_Problems.push_back(current_saas_problem);
    SaaSs.push_back(current_SaaS_ptr);
  }

  IaaS_Problem system_of_IaaS( iaas, SaaSs, omega, rho, delta, N );
  std::pair< IaaS_Problem, std::vector<SaaS_Problem> > result = std::make_pair(system_of_IaaS, SaaS_Problems);

  return result;
}
