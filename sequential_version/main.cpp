#include "SaaS.hpp"
#include "application.hpp"
#include "WS.hpp"
#include "SaaS_Problem.hpp"
#include "Set_System.hpp"
#include "IaaS.hpp"
#include "Game.hpp"
#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include <ios>
#include <time.h>
//#include "gnuplot-iostream.h"

// you have to pass options to run this program:
// r = random run
// e = execute ( in this case you have to provide also the name of the prameters files )
// if you don't specify anything the program just generates the parameters randomly

int main( int argc, char * argv[] )
{
  clock_t start,end;
  double tempo;
  start=clock();
  if( *argv[1] == 'r' )
  {
    std::string config_file_name = argv[2];
    Game game( 1996, config_file_name);
    game.solve();
  }

  if( *argv[1] == 'e' )
  {
    if( argc < 5 )
    {
      std::cerr << "Provide the files' name" << '\n';
    }
    else
    {
      std::string config_file_name = argv[2];
      std::string global_file_name = argv[3];
      std::string SaaSs_file_name = argv[4];
      std::string WSs_file_name = argv[5];
      Game game( 1996, config_file_name, global_file_name, SaaSs_file_name, WSs_file_name );
      game.solve();
    }
  }

  if ( * argv[1] == 'g')
  {
    std::cout << "Generetion of random parameters " << '\n';
    std::string config_file_name = argv[2];
    Game game( 1996, config_file_name);
  }

  end=clock();
  tempo=((double)(end-start))/CLOCKS_PER_SEC;
  std::cout << tempo<< '\n';
return 0;
}
