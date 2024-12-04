#include <iostream>
#include "Interp4Rotate.hh"

using std::cout;
using std::endl;

extern "C"
{
  AbstractInterp4Command *CreateCmd(void);
  const char *GetCmdName() { return "Rotate"; }
}

/*!
 * \brief
 *
 *
 */
AbstractInterp4Command *CreateCmd(void)
{
  return Interp4Rotate ::CreateCmd();
}

/*!
 *
 */
Interp4Rotate::Interp4Rotate() : _ObjName(""), _Speed_degmS(0), _Rotation_deg(0), _Axis("OX")
{
}

/*!
 *
 */
void Interp4Rotate::PrintCmd() const
{
  /*
   *  Tu trzeba napisać odpowiednio zmodyfikować kod poniżej.
   */
  cout << GetCmdName() << "\t" << _ObjName << "\t" << _Axis << "\t" << _Speed_degmS << "\t" << _Rotation_deg << endl;
}

/*!
 *
 */
const char *Interp4Rotate::GetCmdName() const
{
  return ::GetCmdName();
}

/*!
 *
 */
bool Interp4Rotate::ExecCmd(AbstractScene &rScn, ComChannel &rComChann)
{

    std::cout << "Wywołano komendę ROTATE\n";


  AbstractMobileObj *wObMob = rScn.FindMobileObj((*this)._ObjName.c_str());

  if (wObMob == nullptr)
  {
    std::cerr << "Nie mogę znaleźć obiektu: " << (*this)._ObjName.c_str() << std::endl;
    return false;
  }

  if (this->_Axis == "OX")
  {

    double start = wObMob->GetAng_Pitch_deg();
    double delta_deg = 0;
    double dist_step_deg = (double)_Rotation_deg / N;
    double time_step_us = (((double)this->_Rotation_deg / this->_Speed_degmS) * 1000000) / N;

    for (int i = 0; i < N; ++i)
    {
      wObMob->LockAccess();
      delta_deg += dist_step_deg;
      wObMob->SetAng_Pitch_deg(delta_deg + start);

      {
        std::ostringstream message;
        message << "UpdateObj ";
        message << "Name=" << wObMob->GetName();
        message << " RotXYZ_deg=";
        message << "(" << wObMob->GetAng_Pitch_deg() << ",";
        message << wObMob->GetAng_Roll_deg() << ",";
        message << wObMob->GetAng_Yaw_deg() << ")\n";
        rComChann.LockAccess();
        rComChann.send(message.str().c_str());
        rComChann.UnlockAccess();
      }
      wObMob->UnlockAccess();

      usleep(time_step_us);
    }
  }
  else if (this->_Axis == "OY")
  {
    double start = wObMob->GetAng_Roll_deg();
    double delta_deg = 0;
    double dist_step_deg = (double)_Rotation_deg / N;
    double time_step_us = (((double)this->_Rotation_deg / this->_Speed_degmS) * 1000000) / N;

    for (int i = 0; i < N; ++i)
    {
      wObMob->LockAccess();
      delta_deg += dist_step_deg;
      wObMob->SetAng_Roll_deg(delta_deg + start);

      {
        std::ostringstream message;
        message << "UpdateObj ";
        message << "Name=" << wObMob->GetName();
        message << " RotXYZ_deg=";
        message << "(" << wObMob->GetAng_Pitch_deg() << ",";
        message << wObMob->GetAng_Roll_deg() << ",";
        message << wObMob->GetAng_Yaw_deg() << ")\n";
        rComChann.LockAccess();
        rComChann.send(message.str().c_str());
        rComChann.UnlockAccess();
      }

      wObMob->UnlockAccess();

      usleep(time_step_us);
    }
  }
  else if (this->_Axis == "OZ")
  {
    double start = wObMob->GetAng_Yaw_deg();
    double delta_deg = 0;
    double dist_step_deg = (double)_Rotation_deg / N;
    double time_step_us = (((double)this->_Rotation_deg / this->_Speed_degmS) * 1000000) / N;

    for (int i = 0; i < N; ++i)
    {
      wObMob->LockAccess();
      delta_deg += dist_step_deg;
      wObMob->SetAng_Yaw_deg(delta_deg + start);

      {
        std::ostringstream message;
        message << "UpdateObj ";
        message << "Name=" << wObMob->GetName();
        message << " RotXYZ_deg=";
        message << "(" << wObMob->GetAng_Pitch_deg() << ",";
        message << wObMob->GetAng_Roll_deg() << ",";
        message << wObMob->GetAng_Yaw_deg() << ")\n";
        rComChann.LockAccess();
        rComChann.send(message.str().c_str());
        rComChann.UnlockAccess();
      }

      wObMob->UnlockAccess();

      usleep(time_step_us);
    }
  }

  return true;
}

/*!
 *
 */
bool Interp4Rotate::ReadParams(std::istream &Strm_CmdsList)
{
  Strm_CmdsList >> _ObjName >> _Axis >> _Speed_degmS >> _Rotation_deg;
    std::cout << "Zakończono czytać parametry ROTATE\n";

  /*
   *  Tu trzeba napisać odpowiedni kod.
   */
  return true;
}

/*!
 *
 */
AbstractInterp4Command *Interp4Rotate::CreateCmd()
{
  return new Interp4Rotate();
}

/*!
 *
 */
void Interp4Rotate::PrintSyntax() const
{
  cout << "   Rotate\tNazwaObiektu\tOsObrotu\tSzybkosc[deg/s]\tKatObrotu[deg]" << endl;
}
