#ifndef CUBOID_HH
#define CUBOID_HH

#include "AbstractMobileObj.hh"
#include <sstream>

class Cuboid : public AbstractMobileObj
{
private:
    std::string _Name;
    Vector3D _Shift;
    Vector3D _Scale;
    Vector3D _Rotation;
    Vector3D _Transition;
    Vector3D _Color;
    
public:
    Cuboid(std::string name,\
    Vector3D shift,\
    Vector3D scale,\
    Vector3D rotation,\
    Vector3D transition,\
    Vector3D color);
    ~Cuboid();

    double GetAng_Roll_deg() const;
    double GetAng_Pitch_deg() const;
    double GetAng_Yaw_deg() const;
    void SetAng_Roll_deg(double Ang_Roll_deg);
    void SetAng_Pitch_deg(double Ang_Pitch_deg);
    void SetAng_Yaw_deg(double Ang_Yaw_deg);
    const Vector3D & GetPositoin_m() const;
    void SetPosition_m(const Vector3D &rPos);
    void SetName(const char* sName);
    const std::string & GetName() const;
    std::string GetParams() const;
};


#endif