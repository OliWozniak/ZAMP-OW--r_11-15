#include "Cuboid.hh"

Cuboid::Cuboid(std::string name,
               Vector3D shift,
               Vector3D scale,
               Vector3D rotation,
               Vector3D transition,
               Vector3D color)
{

    (*this)._Name = name;
    (*this)._Shift = shift;
    (*this)._Scale = scale;
    (*this)._Rotation = rotation;
    (*this)._Transition = transition;
    (*this)._Color = color;
}

Cuboid::~Cuboid()
{
}

double Cuboid::GetAng_Roll_deg() const
{
    return (*this)._Rotation[0];
}

double Cuboid::GetAng_Pitch_deg() const
{
    return (*this)._Rotation[1];
}

double Cuboid::GetAng_Yaw_deg() const
{
    return (*this)._Rotation[2];
}

void Cuboid::SetAng_Roll_deg(double Ang_Roll_deg)
{
    (*this)._Rotation[0] = Ang_Roll_deg;
}

void Cuboid::SetAng_Pitch_deg(double Ang_Pitch_deg)
{
    (*this)._Rotation[1] = Ang_Pitch_deg;
}

void Cuboid::SetAng_Yaw_deg(double Ang_Yaw_deg)
{
    (*this)._Rotation[2] = Ang_Yaw_deg;
}

const Vector3D &Cuboid::GetPositoin_m() const
{
    return (*this)._Transition;
}

void Cuboid::SetPosition_m(const Vector3D &rPos)
{
    (*this)._Transition = rPos;
}

void Cuboid::SetName(const char *sName)
{
    (*this)._Name = sName;
}

const std::string &Cuboid::GetName() const
{
    return (*this)._Name;
}

std::string Cuboid::GetParams() const
{
    std::ostringstream oss;

    oss << "Name=" << _Name << " ";
    oss << "Shift=" << _Shift << " ";
    oss << "Scale=" << _Scale << " ";
    oss << "RotXYZ_deg=" << _Rotation << " ";
    oss << "Trans_m=" << _Transition << " ";
    oss << "RGB=" << _Color;

    return oss.str();
}

void Cuboid::LockAccess()
{
    access.lock();
}

void Cuboid::UnlockAccess()
{
    access.unlock();
}
