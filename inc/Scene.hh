#ifndef SCENE_HH
#define SCENE_HH

#include "AbstractScene.hh"
#include "AbstractMobileObj.hh"

#include <map>
#include <memory>

class Scene : public AbstractScene
{
private:
    std::map<std::string,std::shared_ptr<AbstractMobileObj>> _Set_MobileObj;
public:
    Scene(/* args */);
    ~Scene();

    void AddMobileObj(std::shared_ptr<AbstractMobileObj> pMobObj);
    AbstractMobileObj*  FindMobileObj(const char *sName);
};


#endif