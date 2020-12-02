#ifndef SET_SAAS_PROLEM_H_
#define SET_SAAS_PROLEM_H_


#include "SaaS.hpp"
#include "application.hpp"
#include "WS.hpp"
#include "SaaS_Problem.hpp"
#include "IaaS_Problem.hpp"
#include <algorithm>
#include <cmath>
#include <vector>
#include <memory>
#include <fstream>

class Set_System
{
  private:
    unsigned seed;
    unsigned Number_of_SaaSs;
    unsigned Number_of_apps;
    unsigned Number_of_WSs;
    double time;


    WS set_WS( unsigned);
    application set_application ( unsigned );
    std::shared_ptr<SaaS> set_SaaS( unsigned );
    std::vector<SaaS_Problem> set_SaaS_problem ( void );

  public:

    //constructor
    Set_System(unsigned, const std::string& config_file_name );

    std::pair< IaaS_Problem, std::vector<SaaS_Problem> > set(void); 
    std::pair< IaaS_Problem, std::vector<SaaS_Problem> > set_from_files(const std::string&, const std::string&, const std::string&);
};

// we define a predicate in order to sort in descending order the SaaSs
struct SortFunctor
{
    bool operator()(SaaS_Problem& object1, SaaS_Problem& object2)
    {
      return(object2.get_SaaS()->get_cost_threshold() < object1.get_SaaS()->get_cost_threshold());
    }
};

#endif
