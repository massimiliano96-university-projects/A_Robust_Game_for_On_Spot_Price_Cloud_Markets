#ifndef _SAAS_PROBLEM_H_
#define _SAAS_PROBLEM_H_


#include "SaaS.hpp"
#include "application.hpp"
#include "WS.hpp"
#include <memory>
#include <fstream>


class SaaS_Problem
{
  private:
    std::shared_ptr<SaaS> s;


    //Problem Parameters
    unsigned T;                           // Control time horizon (tipically 1 hour)
    unsigned N;                           // maximum number of VMs that can be executed at the IaaS
    int gamma = 1;                        // maximum number of uncertain parameters
    double rho;                           // price of on flat VMs
    double delta;                         // price of on demand VMs
    double q_j;                           // Maximum fraction of reserved VMs price for on spot VMs for SaaS j
    double eta_j = 0.25;                  // Maximum fraction of total resources allocated as on spot VMs for SaaS provider j
    unsigned R_j;                         // Maximum number of reserved VMs that can be executed for the SaaS j
    double mu_dev = 0.1;                  // uncertainty on mu
    double D_dev  = 0.1;                  // uncertainty on mu
    double objective_function_value = .0; //the value of the objective function
    double rejected_requests = .0;        // the number of rejected requests
    double sigma = 0.0;

    std::vector<double> rejected_requests_vec;
    //vector in which we store the cost thresholds, in order to map changes during the rod
    std::vector<double> cost_thresholds;

    double iterations = 0;

    bool infeasible = false;

    //function needed to compute the parameters B
    std::vector<std::pair<double, double>> set_param_B ( WS &, application& );


    //function needed to compute the maximum deviation of the parameter B
    double compute_B_dev( WS& , WS&, double );

  public:

    //constructor
    SaaS_Problem() = default;
    SaaS_Problem(std::shared_ptr<SaaS> saas, double q ,double r):
      s(saas), q_j(q), R_j(r)
      {
        double threshold = 0.5*q_j*rho;
        s -> set_cost_threshold( threshold );                    // we set an initial value for cost threshold
        cost_thresholds.push_back(threshold);
      };

    SaaS_Problem(std::shared_ptr<SaaS> saas, unsigned t, unsigned n, int g, double r, double del, double q, double e, unsigned R, double m_d, double d_d):
      s(saas), T(t), N(n), gamma(g), rho(r), delta(del), q_j(q), eta_j(e), R_j(R) ,mu_dev(m_d), D_dev(d_d)
      {
        double threshold = 0.5*q_j*rho;
        s -> set_cost_threshold( threshold );                    // we set an initial value for cost threshold
        cost_thresholds.push_back(threshold);
      };

    // methods

    void solve( void );                                         // this method solve the SaaS_Problem

    void solve_ideal();

    void solve_minimal();

    bool check( void );                                         // this method check if the SaaS's requests ( in terms of on_spot VMs ) have been satisfied

    void print( std::ofstream& );                               // this method print the results

    void update( void );                                        // this method update the number of on_spot VMs before the final iteration

    void rounding( void );                                      // this method perform the rounding of the number of the VMs

    void rounding_ideal();

    void reset_desired( void );                                 // this method reset the number of desired_on_spot, this is called before the last call of rounding method

    void reset_given_on_spot( void );                           // this method reset the number of given_on_spot, this is used from the IaaS_Problem, in the funcion
                                                                // reset_SaaSs_given_on_spot at the beginning of solve_greedy method
    void reset_on_spot( void );

    // getters
    std::shared_ptr<SaaS> get_SaaS( void );

    double get_rho( void );

    double get_delta( void );

    unsigned get_N( void );

    unsigned get_T( void );

    double get_cost_threshold( void );

    int get_gamma( void );

    double get_mu_dev( void );

    double get_D_dev( void );

    double get_q_j( void );

    double get_eta_j( void );

    unsigned get_R_j( void );

    std::vector<double> get_rejected_requests_vec( void );

    double get_iterations( void );

    double get_obj_function( void );

    double get_total_rejected( void );

    double get_sigma(){return sigma;}

    bool get_infeasible(){return infeasible;}

    //  setter
    void set_infeasible( bool current_infeasible){infeasible = current_infeasible;}

    void set_N ( unsigned );

    void set_sigma( unsigned );

    void set_cost_threshold( void );                            // this method update the cost_threshold augmentig it by the 10%

    void set_sigma_j( double value )
    {
      s -> set_cost_threshold( value);
      cost_thresholds.push_back( value );
    }

    void set_T( unsigned );

    void set_gamma( int );

    void set_rho( double );

    void set_delta( double );

    void set_mu_dev( double );

    void set_D_dev( double );

    void set_q_j( double );

    void set_R_j( unsigned );

    void set_eta_j( double );

    void set_sigma( double current_sigma){sigma = current_sigma;}
  };


#endif
