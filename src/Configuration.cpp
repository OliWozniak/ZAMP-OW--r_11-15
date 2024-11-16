#include "Configuration.hh"

void Configuration::addObject(std::shared_ptr<Cuboid> obj){
    (*this)._ObjList.push_back(obj);
}

void Configuration::addLib(std::string lib){
    (*this)._LibList.push_back(lib);
}

std::shared_ptr<Cuboid> Configuration::popObject(){
    std::shared_ptr<Cuboid> popped = (*this)._ObjList.front();
    (*this)._ObjList.pop_front();
    return popped;
}

std::string Configuration::popLib(){
    std::string popped= (*this)._LibList.front();
    (*this)._LibList.pop_front();
    return  popped;
}

int Configuration::sizeLib() const{
    return (*this)._LibList.size();
}

int Configuration::sizeObj() const{
    return (*this)._ObjList.size();
}