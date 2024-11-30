#include <iostream>
#include <dlfcn.h>
#include <cassert>
#include <fstream>
#include "AbstractInterp4Command.hh"
#include "LibInterface.hh"
#include "Set4LibInterfaces.hh"
#include "ComChannel.hh"


#include <iostream>
#include <dlfcn.h>
#include <cassert>
#include <map>
#include <memory>
#include <fstream>  // Dodaj ten nagłówek dla std::ifstream
#include <iostream>
#include <list>


#include "xmlinterp.hh"
#include "AbstractInterp4Command.hh"
#include "LibInterface.hh"
#include "ProgramInterpreter.hh"


#include <xercesc/sax2/SAX2XMLReader.hpp>
#include <xercesc/sax2/XMLReaderFactory.hpp>
#include <xercesc/sax2/DefaultHandler.hpp>
#include <xercesc/util/XMLString.hpp>



using namespace std;
using namespace xercesc;


void _macro(std::string filename)
{
  std::string cmd = "cpp -P "+filename+" -o "+filename+".tmp";
  system(cmd.c_str());
}

int main(int argc, const char *argv[])
{

  Configuration   Config;

  std::string nazwa_program_dzialan;
  std::string nazwa_plik_wykonawczy;
  std::string nazwa_config;

  ProgramInterpreter Interp;

  if(argc < 2){
    std::cout << "Nie poprawna liczba parametrów wejsciowych" << std::endl;
    return 1;
  }

  nazwa_program_dzialan = argv[1];
  nazwa_config = std::string(argv[2]);

  _macro(nazwa_program_dzialan);

  nazwa_plik_wykonawczy = nazwa_program_dzialan + ".tmp";

  std::cout << nazwa_plik_wykonawczy << std::endl;

  if (!Interp.Read_XML_Config(nazwa_config.c_str())) return 1;

  std::cout << "\n\n\n\nZakończono wgrywanie XML\n\n\n\n";
  if (!Interp.ExecProgram(nazwa_plik_wykonawczy.c_str())) return 1;

  std::cout << "\n\n\n\nKoniec programu\n\n\n\n";
  

  return 0;
}
