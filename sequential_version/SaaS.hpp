#ifndef SAAS_H
#define SAAS_H


#include "application.hpp"
#include "WS.hpp"
#include <vector>


class SaaS
{
  private:
    std::vector<application> applications;

// VARIABLES:
    std::vector<double> on_flat;
    std::vector<double> on_demand;
    std::vector<double> on_spot;
    std::vector<double> desired_on_spot;
    // each application corresponds to an index of the vector

    std::vector< double > given_on_spot;    // this vector is updated at each iteration, and it contains the on_spot VMs given by the IaaS,
                                            // we need it for the check function

    double cost_threshold;  //sigma_j_bar

    std::vector<double> response_time;
    std::vector<double> throughput;

    unsigned find_application( const application &) const;

  public:

// constructors:
    SaaS() = default;
    SaaS(std::vector<application> &v): applications(v)
    {
      unsigned size = v.size();
      on_flat.resize(size);
      on_demand.resize(size);
      on_spot.resize(size);
      desired_on_spot.resize(size);
      given_on_spot.resize(size);

    };

    void compute_response_time( void );

  //getters:
    const std::vector<application>& get_applications (void) const;

    std::size_t get_size( void );

    double get_on_flat (const application &) const;

    double get_on_demand (const application &) const;

    double get_on_spot (const application &) const;

    double get_desired_on_spot (const application &) const;

    double get_given_on_spot(const application &) const;

    double get_cost_threshold ( void ) const;

    double get_throughput( unsigned ) const ;

    double get_response_time( unsigned ) const;

  //setters:
    void  add_application(  const application &);

    void set_on_flat ( const application & , double);

    void set_on_demand ( const application &  , double);

    void set_on_spot ( const application &  , double);

    void set_desired_on_spot ( const application & , double);

    void set_given_on_spot ( const application & , double);

    void set_cost_threshold (double);

    void set_throughput(double);

  //oprator<
    bool operator< ( const SaaS & );

};
#endif
