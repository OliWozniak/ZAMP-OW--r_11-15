#include <iostream>
#include "Interp4Move.hh"


using std::cout;
using std::endl;


extern "C" {
  AbstractInterp4Command* CreateCmd(void);
  const char* GetCmdName() { return "Move"; }
}




/*!
 * \brief
 *
 *
 */
AbstractInterp4Command* CreateCmd(void)
{
  return Interp4Move::CreateCmd();
}


/*!
 *
 */
Interp4Move::Interp4Move(): _ObjName(""), _Speed_mmS(0), _Distance_m(0)
{}


/*!
 *
 */
void Interp4Move::PrintCmd() const
{
  /*
   *  Tu trzeba napisać odpowiednio zmodyfikować kod poniżej.
   */
  cout << GetCmdName() << "\t" << _ObjName << "\t" << _Speed_mmS  << "\t" << _Distance_m << endl;
}


/*!
 *
 */
const char* Interp4Move::GetCmdName() const
{
  return ::GetCmdName();
}


/*!
 *
 */
bool Interp4Move::ExecCmd( AbstractScene &rScn,ComChannel &rComChann)
{
  Vector3D new_trans;
  /*
   *  Tu trzeba napisać odpowiedni kod.
   */

  std::cout << "Wywołano komendę MOVE\n";

  AbstractMobileObj* wObMob = rScn.FindMobileObj((*this)._ObjName.c_str());

    if( wObMob == nullptr )
    {
        std::cerr<<"Nie mogę znaleźć obiektu: "<<(*this)._ObjName.c_str()<<std::endl;
        return false;
    }


  Vector3D startPos = wObMob->GetPositoin_m();
  Vector3D newPos;
    double startRoll = wObMob->GetAng_Roll_deg();
    double startPitch = wObMob->GetAng_Pitch_deg();
    double startYaw = wObMob->GetAng_Yaw_deg();
    double delta_x_m, delta_y_m, delta_z_m;
    delta_x_m = delta_y_m = delta_z_m = 0;
    double dist_step_m = (double)_Distance_m/N;
    double time_step_us = (((double)_Distance_m/this->_Speed_mmS)*1000000)/N;

    for(int i = 0; i<N; ++i)
    {  
        delta_x_m += dist_step_m*cos(startPitch*M_PI/180)*cos(startYaw*M_PI/180);
        delta_y_m += dist_step_m*(cos(startRoll*M_PI/180)*sin(startYaw*M_PI/180) + cos(startYaw*M_PI/180)*sin(startPitch*M_PI/180)*sin(startRoll*M_PI/180));
        delta_z_m += dist_step_m*(sin(startRoll*M_PI/180)*sin(startYaw*M_PI/180) - cos(startRoll*M_PI/180)*cos(startYaw*M_PI/180)*sin(startPitch*M_PI/180));
        newPos[0]=delta_x_m+startPos[0];
        newPos[1]=delta_y_m+startPos[1];
        newPos[2]=delta_z_m+startPos[2];
        wObMob->LockAccess();
        wObMob->SetPosition_m(newPos);
        {
         std::string message;
         message = "UpdateObj ";
         message += "Name=" + wObMob->GetName();
         std::ostringstream position;
         position << wObMob->GetPositoin_m();
         message += " Trans_m=" + position.str() +"\n";
         rComChann.LockAccess();
         rComChann.send(message.c_str());
         rComChann.UnlockAccess();
        }

        wObMob->UnlockAccess();

        usleep(time_step_us);
    }
    

  return true;
}

/*!
 *
 */
bool Interp4Move::ReadParams(std::istream& Strm_CmdsList)
{
  Strm_CmdsList >> _ObjName >> _Speed_mmS >> _Distance_m;
    std::cout << "Zakończono czytać parametry MOVE\n";
  /*
   *  Tu trzeba napisać odpowiedni kod.
   */
  return true;
}


/*!
 *
 */
AbstractInterp4Command* Interp4Move::CreateCmd()
{
  return new Interp4Move();
}


/*!
 *
 */
void Interp4Move::PrintSyntax() const
{
  cout << "   Move  NazwaObiektu  Szybkosc[m/s]  DlugoscDrogi[m]" << endl;
}
