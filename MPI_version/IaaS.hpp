#ifndef  _IAAS_H_
#define  _IAAS_H_

#include <vector>

class IaaS
{
  private:

    double sigma;                  //price for on spot VMs

    std::vector< unsigned > s_a;   // here we store the number of on-spot VMs for each application a in A

  public:

    //constructor
    IaaS() = default;

    //getters
    const double get_sigma( void ) const;

    const std::vector< unsigned > get_s_a( void ) const;

    //setters
    void set_sigma( const double );

    void set_s_a( unsigned, unsigned ); // input : two unsigned, the first one is the index, the second the number of  VMs

    //print
    void print ( void );
};
#endif
