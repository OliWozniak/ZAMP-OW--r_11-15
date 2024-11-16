#include "Scene.hh"

Scene::Scene(/* args */){}

Scene::~Scene(){}

void Scene::AddMobileObj(std::shared_ptr<AbstractMobileObj> pMobObj){
    (*this)._Set_MobileObj[pMobObj.get()->GetName()] = pMobObj;
}


AbstractMobileObj*  Scene::FindMobileObj(const char *sName){

    if ((*this)._Set_MobileObj.find(sName) == (*this)._Set_MobileObj.end()) {
      std::cerr << "!!! Nieznany obiekt: " << sName << std::endl;
        throw; // kiedyś coś napiszę
    }

    return (*this)._Set_MobileObj[sName].get();
}