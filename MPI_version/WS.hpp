#ifndef WEB_SERVICE_H
#define WEB_SERVICE_H

#include <iostream>
#include <vector>


class WS
{
private:

  double LAMBDA_a_w = 0;        // overall prediction of the arrival rate for WS w of application a

  double lambda_a_w = 0;        // minimum arrival rate to be guaranteed for WS w of application a

  double D_a_w;                 // Queueing delay for executing WS class w of application a

  double mu_a_w;                // Maximum service rate for executing WS class w of application a

  double R_bar_a_w;             // WS w average response time threshold

  double nu_a_w;                // Penalty for rejecting a single WS w request for application a

public:

  //constructor
  WS() = default;
  WS( double, double, double,double, double, double);
  WS( double, double,double, double);

  //getters
  double get_LAMBDA_a_w(void);

  double get_lambda_a_w(void);

  double get_D_a_w(void);

  double get_mu_a_w(void);

  double get_R_bar_a_w(void);

  double get_nu_a_w(void);

  //setters
  void set_LAMBDA_a_w( double value );

  void set_lambda_a_w( double value );

  void set_D_a_w( double value );

  void set_mu_a_w( double value );

  void set_R_bar_a_w( double value );

  void set_nu_a_w( double value );

  //methods
  bool operator== (const WS &) const;

  void print ( void );

};
#endif
