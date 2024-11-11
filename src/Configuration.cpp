#include "Configuration.hh"

void Configuration::addObject(std::shared_ptr<Cuboid> obj){
    (*this)._ObjList.push_back(obj);
}

void Configuration::addLib(std::shared_ptr<std::string> lib){
    (*this)._LibList.push_back(lib);
}

std::shared_ptr<Cuboid> Configuration::popObject(){
    std::shared_ptr<Cuboid> popped = (*this)._ObjList.back();
    (*this)._ObjList.pop_back();
    return popped;
}

std::shared_ptr<std::string> Configuration::popLib(){
    std::shared_ptr<std::string> popped= (*this)._LibList.back();
    (*this)._LibList.pop_back();
    return  popped;
}

int Configuration::sizeLib() const{
    return (*this)._LibList.size();
}

int Configuration::sizeObj() const{
    return (*this)._ObjList.size();
}