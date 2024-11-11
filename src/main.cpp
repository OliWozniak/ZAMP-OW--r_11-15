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



/*!
 * Czyta z pliku opis poleceń i dodaje je do listy komend,
 * które robot musi wykonać.
 * \param sFileName - (\b we.) nazwa pliku z opisem poleceń.
 * \param CmdList - (\b we.) zarządca listy poleceń dla robota.
 * \retval true - jeśli wczytanie zostało zrealizowane poprawnie,
 * \retval false - w przeciwnym przypadku.
 */

/*bool ReadFile(const char* sFileName, Configuration &rConfig)
{
   try {
            XMLPlatformUtils::Initialize();
   }
   catch (const XMLException& toCatch) {
            char* message = XMLString::transcode(toCatch.getMessage());
            cerr << "Error during initialization! :\n";
            cerr << "Exception message is: \n"
                 << message << "\n";
            XMLString::release(&message);
            return 1;
   }

   SAX2XMLReader* pParser = XMLReaderFactory::createXMLReader();

   pParser->setFeature(XMLUni::fgSAX2CoreNameSpaces, true);
   pParser->setFeature(XMLUni::fgSAX2CoreValidation, true);
   pParser->setFeature(XMLUni::fgXercesDynamic, false);
   pParser->setFeature(XMLUni::fgXercesSchema, true);
   pParser->setFeature(XMLUni::fgXercesSchemaFullChecking, true);

   pParser->setFeature(XMLUni::fgXercesValidationErrorAsFatal, true);

   DefaultHandler* pHandler = new XMLInterp4Config(rConfig);
   pParser->setContentHandler(pHandler);
   pParser->setErrorHandler(pHandler);

   try {
     
     if (!pParser->loadGrammar("config/config.xsd",
                              xercesc::Grammar::SchemaGrammarType,true)) {
       cerr << "!!! Plik grammar/actions.xsd, '" << endl
            << "!!! ktory zawiera opis gramatyki, nie moze zostac wczytany."
            << endl;
       return false;
     }
     pParser->setFeature(XMLUni::fgXercesUseCachedGrammarInParse,true);
     pParser->parse(sFileName);
   }
   catch (const XMLException& Exception) {
            char* sMessage = XMLString::transcode(Exception.getMessage());
            cerr << "Informacja o wyjatku: \n"
                 << "   " << sMessage << "\n";
            XMLString::release(&sMessage);
            return false;
   }
   catch (const SAXParseException& Exception) {
            char* sMessage = XMLString::transcode(Exception.getMessage());
            char* sSystemId = xercesc::XMLString::transcode(Exception.getSystemId());

            cerr << "Blad! " << endl
                 << "    Plik:  " << sSystemId << endl
                 << "   Linia: " << Exception.getLineNumber() << endl
                 << " Kolumna: " << Exception.getColumnNumber() << endl
                 << " Informacja: " << sMessage 
                 << endl;

            XMLString::release(&sMessage);
            XMLString::release(&sSystemId);
            return false;
   }
   catch (...) {
            cout << "Zgloszony zostal nieoczekiwany wyjatek!\n" ;
            return false;
   }

   delete pParser;
   delete pHandler;
   return true;
}*/


void _macro(std::string filename)
{
  std::string cmd = "cpp -P "+filename+" -o "+filename+".tmp";
  system(cmd.c_str());
}

int main(int argc, const char *argv[])
{

  std::string nazwa_program_dzialan;
  std::string nazwa_plik_wykonawczy;
  std::string nazwa_config;

  ProgramInterpreter Interp;

  Configuration   Config;

  ComChannel Comm;

  Comm.open_connection();

    const char *sConfigCmds =
"Clear\n"
"AddObj Name=Podstawa1 RGB=(20,200,200) Scale=(4,2,1) Shift=(0.5,0,0) RotXYZ_deg=(0,-45,20) Trans_m=(-1,3,0)\n"
"AddObj Name=Podstawa1.Ramie1 RGB=(200,0,0) Scale=(3,3,1) Shift=(0.5,0,0) RotXYZ_deg=(0,-45,0) Trans_m=(4,0,0)\n"
"AddObj Name=Podstawa1.Ramie1.Ramie2 RGB=(100,200,0) Scale=(2,2,1) Shift=(0.5,0,0) RotXYZ_deg=(0,-45,0) Trans_m=(3,0,0)\n"       
"AddObj Name=Podstawa2 RGB=(20,200,200) Scale=(4,2,1) Shift=(0.5,0,0) RotXYZ_deg=(0,-45,0) Trans_m=(-1,-3,0)\n"
"AddObj Name=Podstawa2.Ramie1 RGB=(200,0,0) Scale=(3,3,1) Shift=(0.5,0,0) RotXYZ_deg=(0,-45,0) Trans_m=(4,0,0)\n"
"AddObj Name=Podstawa2.Ramie1.Ramie2 RGB=(100,200,0) Scale=(2,2,1) Shift=(0.5,0,0) RotXYZ_deg=(0,-45,0) Trans_m=(3,0,0)\n";

  Comm.send(sConfigCmds);

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

  ifstream process_instruction(nazwa_plik_wykonawczy);
  if (!process_instruction.is_open()) {
        cerr << "Error opening the file!";
        return 1;
    }
  std::string command;

  Set4LibInterfaces interfaces;

  if(!interfaces.load_libraries()){
    return 1;
  }

  while (true) {
    
    if(! (process_instruction >> command)){
      break;
    }

    if(!interfaces.print_command(command, process_instruction)){
      return 1;
    }

  }

  return 0;
}
