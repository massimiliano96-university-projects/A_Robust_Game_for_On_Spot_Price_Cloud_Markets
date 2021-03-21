#include "SaaS_Problem.hpp"
#include "SaaS.hpp"
#include "application.hpp"
#include "WS.hpp"
#include "gurobi_c++.h"
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>
#include <memory>

std::vector<std::pair<double, double>> SaaS_Problem::set_param_B ( WS & w, application& app)
{
  std::vector <std::pair<double, double>> result; // we will return a vector of paris, we will have as pair.first the nominal value B_bar_a_w_l and as pair.second B_dev
  double A_a_w = w.get_R_bar_a_w() - w.get_D_a_w(); // first compute A_a_w (application a, web service w)
  std::vector<double> B;
  double B_dev = 0.0;
  for( unsigned l = 0; l < app.get_size(); l++) //cycle among all the web services of the application
  {
    auto web_service_l = app.get_web_services()[l];
    double B_a_x_l_bar = A_a_w*w.get_mu_a_w()/((1-A_a_w*w.get_mu_a_w())*web_service_l.get_mu_a_w());
    B_dev = compute_B_dev( w, web_service_l, B_a_x_l_bar );


    //std::cout << "B_bar = "<< B_a_x_l_bar << '\n'; // debugging
    //std::cout << "B_dev = "<< B_dev << '\n'; //debugging


    result.push_back(std::make_pair( B_a_x_l_bar ,  B_dev));
  }
  return result;
}


// here we compute the maximal deviation from the nominal value of B_bar_a_w_l
double SaaS_Problem::compute_B_dev( WS & web_service_w, WS & web_service_l, double B_bar_a_w_w)
{

  double B_dev = .0;

  if (web_service_w == web_service_l )
  {
    double b1 = std::abs( B_bar_a_w_w - ((web_service_w.get_R_bar_a_w() - (1 + D_dev) * web_service_w.get_D_a_w())*(1 + mu_dev)*web_service_w.get_mu_a_w())/
                        ((1 - ((web_service_w.get_R_bar_a_w() - (1 + D_dev)) * web_service_w.get_D_a_w()*(1 + mu_dev)*web_service_w.get_mu_a_w()))
                        * (1 + mu_dev)*web_service_w.get_mu_a_w()));

    double b2 = std::abs( B_bar_a_w_w - ((web_service_w.get_R_bar_a_w() - (1 - D_dev) * web_service_w.get_D_a_w())*(1 + mu_dev)*web_service_w.get_mu_a_w())/
                        ((1 - ((web_service_w.get_R_bar_a_w() - (1 - D_dev)) * web_service_w.get_D_a_w()*(1 + mu_dev)*web_service_w.get_mu_a_w()))
                        * (1 + mu_dev)*web_service_w.get_mu_a_w()));

    double b3 = std::abs( B_bar_a_w_w - ((web_service_w.get_R_bar_a_w() - (1 + D_dev) * web_service_w.get_D_a_w())*(1 - mu_dev)*web_service_w.get_mu_a_w())/
                        ((1 - ((web_service_w.get_R_bar_a_w() - (1 + D_dev)) * web_service_w.get_D_a_w()*(1 - mu_dev)*web_service_w.get_mu_a_w()))
                        * (1 - mu_dev)*web_service_w.get_mu_a_w()));

    double b4 = std::abs( B_bar_a_w_w - ((web_service_w.get_R_bar_a_w() - (1 - D_dev) * web_service_w.get_D_a_w())*(1 - mu_dev)*web_service_w.get_mu_a_w())/
                        ((1 - ((web_service_w.get_R_bar_a_w() - (1 - D_dev)) * web_service_w.get_D_a_w()*(1 - mu_dev)*web_service_w.get_mu_a_w()))
                        * (1 - mu_dev)*web_service_w.get_mu_a_w()));

    B_dev = std::max( {b1, b2, b3, b4} );
  }
  else
  {
    double A_a_w = web_service_w.get_R_bar_a_w() - web_service_w.get_D_a_w();

    double b1 = std::abs( A_a_w*web_service_w.get_mu_a_w()/( 1 - A_a_w * web_service_w.get_mu_a_w()) -
                        ((web_service_w.get_R_bar_a_w() - (1 + D_dev) * web_service_w.get_D_a_w())*(1 + mu_dev)*web_service_w.get_mu_a_w())/
                        (1 - ((web_service_w.get_R_bar_a_w() - (1 + D_dev)) * web_service_w.get_D_a_w()*(1 + mu_dev)*web_service_w.get_mu_a_w())));

    double b2 = std::abs( A_a_w*web_service_w.get_mu_a_w()/( 1 - A_a_w *web_service_w.get_mu_a_w()) -
                        ((web_service_w.get_R_bar_a_w() - (1 - D_dev) * web_service_w.get_D_a_w())*(1 + mu_dev)*web_service_w.get_mu_a_w())/
                        (1 - ((web_service_w.get_R_bar_a_w() - (1 - D_dev)) * web_service_w.get_D_a_w()*(1 + mu_dev)*web_service_w.get_mu_a_w())));

    double b3 = std::abs( A_a_w*web_service_w.get_mu_a_w()/( 1 - A_a_w * web_service_w.get_mu_a_w()) -
                        ((web_service_w.get_R_bar_a_w() - (1 + D_dev)) * web_service_w.get_D_a_w()*(1 - mu_dev)*web_service_w.get_mu_a_w())/
                        (1 - ((web_service_w.get_R_bar_a_w() - (1 + D_dev)) * web_service_w.get_D_a_w()*(1 - mu_dev)*web_service_w.get_mu_a_w())));

    double b4 = std::abs( A_a_w*web_service_w.get_mu_a_w()/( 1 - A_a_w * web_service_w.get_mu_a_w()) -
                        ((web_service_w.get_R_bar_a_w() - (1 - D_dev)) * web_service_w.get_D_a_w()*(1 - mu_dev)*web_service_w.get_mu_a_w())/
                        (1 - ((web_service_w.get_R_bar_a_w() - (1 - D_dev)) * web_service_w.get_D_a_w()*(1 - mu_dev)*web_service_w.get_mu_a_w())));

    B_dev = 1 / (( 1 - mu_dev ) * web_service_l.get_mu_a_w()) * std::max( {b1, b2, b3, b4} );

  }
  return B_dev;
}


void SaaS_Problem::solve(void)
{
  try
  {

    GRBEnv env = GRBEnv(); // create a gurobi environment

    GRBModel model = GRBModel(env); //create an empty model

    GRBLinExpr obj = 0.0; // create a linear expression to construct my objective function

    // Create variables

    GRBLinExpr total_on_flat = 0;   // create a linear expression which will be the sum of all on_flat VMs
    GRBLinExpr total_VM = 0;        // create a linear expression which will be the sum of all VMs
    std::vector<GRBVar> x_v;        // here we store the variables X_a_w because we need them for the computation of the parameters B

    std::vector<GRBVar> alpha_v;    // here we store all the values of alpha, we need it just for debugging
    std::vector<GRBVar> beta_v;     // here we store all the values of beta, we need it just for debugging
    std::vector<GRBVar> y_v;        // here we store the variables the number of rejacted requests

    //std::vector<GRBLinExpr> bx; // for debugging

    double total_on_spot = 0.0;

    // setting the variables and the objective function
    for(unsigned i = 0; i < s -> get_size(); i++)                                      // cicle over all the applications of the SaaS
    {

      auto app = s -> get_applications()[i];                                           // ith application

      total_on_spot += s -> get_on_spot(app);

      std::string r_a = "r" + std::to_string(i);                                       // create the name for the variables
      std::string d_a = "d" + std::to_string(i);

      GRBVar r  = model.addVar(0.0, GRB_INFINITY, 0.0  , GRB_CONTINUOUS, r_a);         // creating r_a
      GRBVar d  = model.addVar(0.0, GRB_INFINITY, 0.0  , GRB_CONTINUOUS, d_a);         // creating d_a

      obj += rho * r;                                                                  // adding r_a to the objecting funcion
      obj += delta * d;                                                                // adding d_a to the objecting funcion

      total_on_flat += r;                                                              // updating the sum of all the on flat VMs
      total_VM += ( r + d );                                                           // updating the sum of all the VMs

      /*
       I have to split the cicle over the WS of the application i, beacuse first i have to store the variables X_a_w and epsilon
       and then use them to add the robust constraints
      */

      for(unsigned j = 0; j < app.get_size(); j++)                                                  // cicle over all the WSs of the application i
      {

        auto web_service = app.get_web_services()[j];                                               // j-th WS of the i-th application

        GRBVar y = model.addVar(0.0, GRB_INFINITY, 0.0, GRB_CONTINUOUS);                            // creating y_a_w

        obj += (T * web_service.get_nu_a_w() * y);                                                  // adding it to the objecting funcion

        GRBVar x = model.addVar(0.0, GRB_INFINITY, 0.0  , GRB_CONTINUOUS);                          // creating X_a_w

        model.addConstr( x >= web_service.get_lambda_a_w() + 1);                                       // X_a_w >= lambda_a_w
        model.addConstr( y + x >= web_service.get_LAMBDA_a_w() );                                   // X_a_w + y_a_w >= LAMBDA_a_w


        y_v.push_back(y);                                                                           // store y_a_w in y_v
        x_v.push_back(x);                                                                           // store X_a_w in x_v

      }

      // here we add the robust constraints
      for(unsigned j = 0; j < app.get_size(); j++)                                       // cicle over all the WSs of the application i
      {
        auto web_service = app.get_web_services()[j];                                    // j-th WS of the i-th application

        GRBLinExpr BX = 0.0;                                                             // linear expression = sum of ( B_bar_a_w_l * X_a_l )

        auto  B_a_w = set_param_B(web_service, app);                                     // computing the parameters B_bar and B_hat using the function set_param_B

        GRBVar alpha = model.addVar(0.0, GRB_INFINITY, 0.0  , GRB_CONTINUOUS);           // creating alpha_a_w
        alpha_v.push_back(alpha);


        GRBLinExpr betas = 0.0;                                                          // linear expression = sum of beta_a_w_l

        for(unsigned l = 0; l < B_a_w.size(); l++)                                       // cicle over WS l of the application i
        {

          GRBVar beta = model.addVar(0.0, GRB_INFINITY, 0.0  , GRB_CONTINUOUS);          // creating beta_a_w_l
          betas += beta;                                                                 // computng the sum of all betas
          beta_v.push_back(beta);

          model.addConstr( alpha + beta >= B_a_w[l].second * x_v[l] );                      // second robust constraint

          BX += B_a_w[l].first * x_v[l];                                                  // computing B_bar_a_w_l * X_a_l

        }

        //bx.push_back(BX);  //for debugging

        model.addConstr( - r - d - s -> get_on_spot(app) - BX + gamma*alpha + betas <= 0 );                     //first robust constraint

      }
    }
    /*
    GRBVar ceil_tot_VM = model.addVar(0, GRB_INFINITY, 0, GRB_INTEGER);
    model.addConstr( total_VM <= ceil_tot_VM );
    model.addConstr( ceil_tot_VM - 1 <= total_VM + 0.000001);

    GRBVar ceil_tot_on_spot = model.addVar(0, GRB_INFINITY, 0, GRB_INTEGER);
    model.addConstr( total_on_spot <= ceil_tot_on_spot );
    model.addConstr( ceil_tot_on_spot - 1 <= total_on_spot + 0.000001);
    */
    model.addConstr( total_on_flat <= R_j);                                     // sum of r_a less or equal to R_j_bar
    //model.addConstr(ceil_tot_VM <= N - ceil_tot_on_spot );                            //  the sum the on flat and on demand VMs lessor equal to N - on_spot VM
    model.addConstr(total_VM <= N - total_on_spot );

    model.setObjective(obj, GRB_MINIMIZE);                                                // set the objective function

    model.optimize();                                                                     // optimize the model

      // for debugging
      for(unsigned i = 0; i < x_v.size(); i++)
      {
        s->set_throughput(x_v[i].get(GRB_DoubleAttr_X) );
      }


    for(unsigned i = 0; i < y_v.size(); i++)                                                               // computing the total number of rejected requests
    {
      rejected_requests += y_v[i].get(GRB_DoubleAttr_X);
      rejected_requests_vec.push_back(i);
    }

    /*
    std::cout << "ceil tot vm = "<< ceil_tot_VM.get(GRB_DoubleAttr_X) << '\n';
    std::cout << "tot vm = "<< total_VM.getValue() << '\n';
    std::cout << "ceil tot spot = "<< ceil_tot_on_spot.get(GRB_DoubleAttr_X) << '\n';
    */

    // Now i store the variables in SaaS
    for(unsigned i = 0; i < (*s).get_size(); i++)                                                          // cicle over all the applications of the SaaS
    {

      application app = (*s).get_applications()[i];

      std::string r_a = "r" + std::to_string(i);                                                           // create the name for the variables
      std::string d_a = "d" + std::to_string(i);

      GRBVar r = model.getVarByName(r_a);
      s->set_on_flat(app, r.get(GRB_DoubleAttr_X));                                           //store on flat VMs for the application i

      GRBVar d = model.getVarByName(d_a);
      s->set_on_demand(app, d.get(GRB_DoubleAttr_X));                                                 //store on demand VMs for the application i
      std::cout << "d = "<< d.get(GRB_DoubleAttr_X)<< '\n';

      s->set_desired_on_spot( app,   eta_j/(1-eta_j) * ( s -> get_on_demand(app) + s -> get_on_flat(app) ));
    }

    objective_function_value = model.get(GRB_DoubleAttr_ObjVal);

    iterations = model.get(GRB_DoubleAttr_IterCount);

  } catch(GRBException e) {
    std::cout << "Error code = " << e.getErrorCode() << std::endl;
    std::cout << e.getMessage() << std::endl;
  } catch (...) {
    std::cout << "Error during optimization" << std::endl;
  }
}

// this function check if the SaaS is satisfied in terms of desired_on_spot VMs
bool SaaS_Problem::check( void )
{
  bool target = true;
  auto apps = s -> get_applications();
  for ( unsigned i = 0; i < s -> get_size(); i++) //iterates among all the applications
  {
    if( s -> get_given_on_spot( apps[i] ) !=  s -> get_desired_on_spot( apps[i] ) ) // if the requirements of an application are not satisfied
    {
      target = false; // then the whole requirements of the SaaS are not satisfied
      break; // hence break the cicle and return false
    }
  }
  return target;
}

std::shared_ptr<SaaS> SaaS_Problem::get_SaaS(void)
{
  return s;
}

double SaaS_Problem::get_rho(void)
{
  return rho;
}

double SaaS_Problem::get_delta(void)
{
  return delta;
}

unsigned SaaS_Problem::get_N(void)
{
  return N;
}

unsigned SaaS_Problem::get_T(void)
{
  return T;
}

double SaaS_Problem::get_cost_threshold( void )
{
  return s -> get_cost_threshold();
}

int SaaS_Problem::get_gamma( void )
{
  return gamma;
}

double SaaS_Problem::get_mu_dev( void )
{
  return mu_dev;
}

double SaaS_Problem::get_D_dev( void )
{
  return D_dev;
}

double SaaS_Problem::get_q_j( void )
{
  return q_j;
}

double SaaS_Problem::get_eta_j( void )
{
  return eta_j;
}

unsigned SaaS_Problem::get_R_j( void )
{
  return R_j;
}

void SaaS_Problem::set_N( unsigned value )
{
  N = value;
}


// this function increase the cost_threshold for the SaaS during the game
void SaaS_Problem::set_cost_threshold( void )
{
  if( q_j*rho - s -> get_cost_threshold() > 0.01 ) // if the actual cost_threshold and the maximum price that SaaS can pay differ for more then 1 cent then:
  {
    double new_threshold = 1.1 * s -> get_cost_threshold(); // increase the cost_threshold by the 10%
    if ( new_threshold < q_j*rho )
    {
      s -> set_cost_threshold( new_threshold ); // and then store the new cost_threshold
      cost_thresholds.push_back( new_threshold ); //map the change
    }
    else
    {
      s -> set_cost_threshold( q_j*rho ); //store the maximum price that SaaS can pay, this will be the last cost_threshold because this SaaS can't pay more
      cost_thresholds.push_back( q_j*rho );
    }
  }
  else // if the actual cost_threshold and the maximum cost are close enough then
  {
    s -> set_cost_threshold( q_j*rho ); //store the maximum price that SaaS can pay, this will be the last cost_threshold because this SaaS can't pay more
    cost_thresholds.push_back( q_j*rho );
  }
}


void SaaS_Problem::update (void)
{
  auto app = s -> get_applications();
  for( unsigned i = 0; i < s -> get_size(); i++ )
  {
    s -> set_on_spot ( app[i] , s -> get_given_on_spot(app[i]) );
  }
}


void SaaS_Problem::rounding( void )
{
  auto apps = s -> get_applications();
  double total_on_flat = .0;

  for(auto app : apps )
  {
    total_on_flat += std::floor( s -> get_on_flat(app) );
    // I need to compute the total number of on_flat VMs in order to respect the constraint : tot_on_flat <= R_j
  }

  double residuals = .0; // we have to compute the total "residual_VM", i.e. the number of VM we need to add in order to satisfy the SaaS's requirements after the floor_rounding
  int count  = 0;
  for(auto app : apps ) // iterate among all the apps
  {
    double on_flat =  s -> get_on_flat(app);
    //std::cout << "on_flat = "<< on_flat << '\n';
    double on_demand = s -> get_on_demand(app);
    //std::cout << "on_demand = "<< on_demand << '\n';
    double desired_on_spot  = s -> get_desired_on_spot(app);
    //std::cout << "desired_on_spot = "<< desired_on_spot << '\n';
    //std::cout << "floor on_demand ="<< std::floor(on_demand) << '\n';

    residuals = on_flat - std::floor(on_flat)  + on_demand - std::floor(on_demand); // compute the residuals + desired_on_spot - std::floor(desired_on_spot)

    /*
    if (residuals < 0.000001) {
      count = std::floor(residuals);
    }
    else
    {
      count = std::ceil(residuals);
    }
    std::cout << "residuals= "<< residuals << '\n';
 // we have to round the residuals to the next integer in order to satisfy the requirements
    std::cout << "count= "<< count << '\n';
    */
    count = std::ceil(residuals);

    s -> set_desired_on_spot( app, std::floor(desired_on_spot)); // we have took the maximum number of desired_on_spot hence I can't have more of them


    //count can be at most 3, hence we check all the possibility
    if( count == 3 && total_on_flat + count <= R_j )
    {
      s -> set_on_flat( app, std::floor(on_flat) + 3 );
      s -> set_on_demand( app, std::floor(on_demand ));
      total_on_flat += 3;
    }
    else if( count == 3 && total_on_flat + 2 <= R_j )
    {
      s -> set_on_flat( app, std::floor(on_flat) + 2 );
      s -> set_on_demand( app, std::floor(on_demand + 1 ));
      total_on_flat += 2;
    }
    else if ( count == 3 )
    {
      s -> set_on_flat( app, std::floor(on_flat));
      s -> set_on_demand( app, std::floor(on_demand ) + 3 );
    }
    if( count == 2 && total_on_flat + count <= R_j )
    {
      s -> set_on_flat( app, std::floor(on_flat) + 2 );
      s -> set_on_demand( app, std::floor(on_demand ));
      total_on_flat += 2;
    }
    else if( count == 2 && total_on_flat + 1 <= R_j )
    {
      s -> set_on_flat( app, std::floor(on_flat) + 1 );
      s -> set_on_demand( app, std::floor(on_demand + 1 ));
      total_on_flat += 1;

    }
    else if ( count == 2 )
    {
      s -> set_on_flat( app, std::floor(on_flat));
      s -> set_on_demand( app, std::floor(on_demand ) + 2 );
    }
    else if( count == 1 && total_on_flat + 1 <= R_j )
    {
      s -> set_on_flat( app, std::floor(on_flat) + 1 );
      s -> set_on_demand( app, std::floor(on_demand ));
      total_on_flat += 1;
    }
    else if( count == 1 )
    {
      s -> set_on_flat( app, std::floor(on_flat));
      s -> set_on_demand( app, std::floor(on_demand ) + 1 );
    }

  }
}


void SaaS_Problem::reset_desired( void )
{
  auto apps = s -> get_applications();
  for( auto & app : apps )
  {
    s -> set_desired_on_spot( app, .0 );
  }
}

void SaaS_Problem::reset_given_on_spot( void )
{
  auto apps = s -> get_applications();
  for( auto & app : apps )
  {
    s -> set_given_on_spot( app, .0 );
  }
}

void SaaS_Problem::print( std::ofstream & ofs )
{

  unsigned count = 0;
  ofs << '\n' << "Results : "<< '\n';
  auto apps = (*s).get_applications();
  count = 0;
  for( auto & app : apps)
  {

    double on_flat_a = (*s).get_on_flat(app);
    double on_demand_a = (*s).get_on_demand(app);
    double on_spot_a = (*s).get_on_spot(app);
    //double desired_on_spot_a = (*s).get_desired_on_spot(app);
    ofs << "- Application " << count <<" : "<< on_flat_a<< " " << on_demand_a << " " <<  on_spot_a <<'\n';
    //ofs << "  desired_on_spot : " << desired_on_spot_a <<'\n';
    ofs << '\n';
    count++;
  }
  double cost = (*s).get_cost_threshold();
  ofs << "cost threshold : " << cost <<'\n';
  ofs << '\n';

  ofs << "objective_function = "<< objective_function_value << '\n';
  ofs << "rejected requests = "<< rejected_requests<< '\n';

  ofs << '\n';
  ofs << "cost_thresholds_history:" <<'\n';
  for(double & t : cost_thresholds )
  {
    ofs << t << " ";
  }
  ofs << '\n';
}

void SaaS_Problem::set_T( unsigned value )
{
  T = value;
}


void SaaS_Problem::set_gamma( int value )
{
  gamma = value;
}

void SaaS_Problem::set_rho( double value )
{
  rho = value;
}

void SaaS_Problem::set_delta( double value )
{
  delta = value;
}

void SaaS_Problem::set_mu_dev( double value )
{
  mu_dev = value;
}

void SaaS_Problem::set_D_dev( double value )
{
  D_dev = value;
}

void SaaS_Problem::set_q_j( double value )
{
  q_j = value;
}

void SaaS_Problem::set_R_j( unsigned value )
{
  R_j = value;
}

void SaaS_Problem::set_eta_j( double value )
{
  eta_j = value;
}

std::vector<double> SaaS_Problem::get_rejected_requests_vec( void )
{
  return rejected_requests_vec;
}

double SaaS_Problem::get_iterations( void )
{
  return iterations;
}

double SaaS_Problem::get_obj_function( void )
{
  return objective_function_value;
}

double SaaS_Problem::get_total_rejected( void )
{
  return rejected_requests;
}
