#ifndef CONFIGURATION_HH
#define CONFIGURATION_HH

#include "Cuboid.hh"

#include <iostream>
#include <list>
#include <string>
#include <memory>

using namespace std;

class Configuration {
  private:
    std::list<std::string> _LibList;
    std::list<std::shared_ptr<Cuboid>> _ObjList;
  public:

    void addObject(std::shared_ptr<Cuboid> obj);
    void addLib(std::string lib);

    shared_ptr<Cuboid> popObject();
    std::string popLib();

    int sizeLib() const;
    int sizeObj() const;
};


#endif