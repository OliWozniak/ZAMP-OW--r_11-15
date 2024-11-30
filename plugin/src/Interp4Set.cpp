#include <iostream>
#include "Interp4Set.hh"

using std::cout;
using std::endl;

extern "C"
{
  AbstractInterp4Command *CreateCmd(void);
  const char *GetCmdName() { return "Set"; }
}

/*!
 * \brief
 *
 *
 */
AbstractInterp4Command *CreateCmd(void)
{
  return Interp4Set::CreateCmd();
}

/*!
 *
 */
Interp4Set::Interp4Set() : _ObjName(""), _X_m(0), _Y_m(0), _Z_m(0), _A_deg(0), _B_deg(0), _G_deg(0)
{
}

/*!
 *
 */
void Interp4Set::PrintCmd() const
{
  /*
   *  Tu trzeba napisać odpowiednio zmodyfikować kod poniżej.
   */
  cout << GetCmdName() << "\t" << _ObjName << "\t" << _X_m << "\t" << _Y_m << "\t" << _Z_m << "\t"
       << _A_deg << "\t" << _B_deg << "\t" << _G_deg << endl;
}

/*!
 *
 */
const char *Interp4Set::GetCmdName() const
{
  return ::GetCmdName();
}

/*!
 *
 */
bool Interp4Set::ExecCmd(AbstractScene &rScn, ComChannel &rComChann)
{
    // std::cout << "Wywołano komendę SET\t" << this->_ObjName.c_str() << this->_A_deg <<std::endl;

  AbstractMobileObj *wObMob = rScn.FindMobileObj(this->_ObjName.c_str());

  if (wObMob == nullptr)
  {
    std::cerr << "Nie mogę znaleźć obiektu: " << this->_ObjName.c_str() << std::endl;
    return false;
  }

  Vector3D new_pos;
  new_pos[0] = _X_m;
  new_pos[1] = _Y_m;
  new_pos[2] = _Z_m;

  std::cout << "Zakończono obliczenia\n";
  
  wObMob->LockAccess();
  wObMob->SetPosition_m(new_pos);
  wObMob->SetAng_Roll_deg(_A_deg);

  wObMob->SetAng_Pitch_deg(_B_deg);
  wObMob->SetAng_Yaw_deg(_G_deg);

  {
    std::string message;
    message = "UpdateObj ";
    message += wObMob->GetParams();
    std::cout << message;
    rComChann.LockAccess();
    rComChann.send(message.c_str());
    rComChann.UnlockAccess();
  }

  wObMob->UnlockAccess();

  return true;
}

/*!
 *
 */
bool Interp4Set::ReadParams(std::istream &Strm_CmdsList)
{
  Strm_CmdsList >> _ObjName >> _X_m >> _Y_m >> _Z_m >> _A_deg >> _B_deg >> _G_deg;
  std::cout << "Zakończono czytać parametry SET\n";
  return true;
}

/*!
 *
 */
AbstractInterp4Command *Interp4Set::CreateCmd()
{
  return new Interp4Set();
}

/*!
 *
 */
void Interp4Set::PrintSyntax() const
{
  cout << "\tSet\tNazwaObiektu\tP_X[m]\tP_Y[m]\tP_Z[m]\tO_X[deg]\tO_Y[deg]\tO_Z[deg]" << endl;
}
