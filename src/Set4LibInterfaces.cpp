#include "../inc/Set4LibInterfaces.hh"

Set4LibInterfaces::Set4LibInterfaces(/* args */)
{
}

Set4LibInterfaces::~Set4LibInterfaces()
{
}

std::shared_ptr<AbstractInterp4Command> Set4LibInterfaces::make_cmd(std::string cmd_name, std::istream& process_instruction){
  if (!(*this).check_library(cmd_name))
  {
    std::cerr << "!!! Nieznane polecenie: " << cmd_name << std::endl;

    return nullptr;
  }

  std::shared_ptr<AbstractInterp4Command> pCmd((*this)[cmd_name].get()->get_cmd());
  if (!pCmd)
  {
    std::cerr << "!!! Blad tworzenia polecenia: " << cmd_name << std::endl;
    return nullptr;
  }

  if (!pCmd.get()->ReadParams(process_instruction))
  {
    std::cerr << "!!! Blad wczytywania parametrow polecenia: " << cmd_name << std::endl;
    return nullptr;
  }

  std::cout <<  "Koniec tworzenia komendy\n";

  return pCmd;
}


bool Set4LibInterfaces::print_command(std::string cmd_name, std::istream &process_instruction)
{

  if (!(*this).check_library(cmd_name))
  {
    std::cerr << "!!! Nieznane polecenie: " << cmd_name << std::endl;

    return false;
  }

  AbstractInterp4Command *pCmd = (*this)[cmd_name].get()->get_cmd();
  if (!pCmd)
  {
    std::cerr << "!!! Blad tworzenia polecenia: " << cmd_name << std::endl;
    return false;
  }

  if (!pCmd->ReadParams(process_instruction))
  {
    std::cerr << "!!! Blad wczytywania parametrow polecenia: " << cmd_name << std::endl;
    delete pCmd;
    return false;
  }

  pCmd->PrintCmd();

  delete pCmd;

  return true;
}

bool Set4LibInterfaces::load_libraries()
{

  std::shared_ptr<LibInterface> Move(new LibInterface());
  std::shared_ptr<LibInterface> Rotate(new LibInterface());
  std::shared_ptr<LibInterface> Pause(new LibInterface());
  std::shared_ptr<LibInterface> Set(new LibInterface());

  if (Move->load_lib("Move") != 0)
  {
    return false;
  }

  if (Rotate->load_lib("Rotate") != 0)
  {
    return false;
  }

  if (Pause->load_lib("Pause") != 0)
  {
    return false;
  }

  if (Set->load_lib("Set") != 0)
  {
    return false;
  }

  (*this)["Move"] = Move;
  (*this)["Rotate"] = Rotate;
  (*this)["Pause"] = Pause;
  (*this)["Set"] = Set;

  return true;
}

bool Set4LibInterfaces::load_library(std::string lib_name)
{
  std::shared_ptr<LibInterface> new_lib(new LibInterface());
  if (new_lib->load_lib(lib_name.c_str()) != 0)
  {
    return false;
  }

  (*this)[lib_name.c_str()] = new_lib;

  return true;
}

bool Set4LibInterfaces::check_library(std::string cmd_name)
{
  if ((*this).find(cmd_name) == (*this).end())
  {
    return false;
  }
  return true;
}

