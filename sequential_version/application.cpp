#include "application.hpp"
#include "WS.hpp"
#include <vector>
#include <limits>

const std::vector<WS> & application::get_web_services(void)
{
    return web_services;
}

std::size_t application::get_size( void )
{
  return web_services.size();
}

unsigned  application::find_WS (const WS & w) const //this function finds the index of a given web service
{
  unsigned count = 0;
  bool found = false;
  while(!found && count != web_services.size())
  {
    if(web_services[count] == w)
      found = true;
    else
      count++;
  }
  if(found == false)
  {
    std::cerr<<"WS not found"<<std::endl;
    return std::numeric_limits<unsigned>::quiet_NaN();
  }
  else
    return count;
}


bool  application::operator==(const application & a) const
{
  return web_services == a.web_services;
}

void application::add_WS( WS& ws )
{
  web_services.push_back(ws);
}
