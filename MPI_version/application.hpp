#ifndef APPLICATION_H
#define APPLICATION_H

#include <vector>
#include "WS.hpp"

class application
{
  private:
    std::vector< WS > web_services; //W_a: vector of web web_services

  public:

    //constructors
    application() = default;
    application ( std::vector< WS > &v): web_services(v) {};
    application ( WS & w):web_services{w} {};

    //getters
    const std::vector<WS>& get_web_services(void);

    std::size_t get_size( void );

    //methods
    unsigned  find_WS (const WS &) const;

    bool  operator==(const application &) const;

    void add_WS( WS & );

};
#endif
