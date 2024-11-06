#include <iostream>
#include <dlfcn.h>
#include <cassert>
#include <fstream>
#include "AbstractInterp4Command.hh"
#include "LibInterface.hh"
#include "Set4LibInterfaces.hh"


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

bool ReadFile(const char* sFileName, Configuration &rConfig)
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
}


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

  Configuration   Config;


  if(argc < 2){
    std::cout << "Nie poprawna liczba parametrów wejsciowych" << std::endl;
    return 1;
  }

  nazwa_program_dzialan = argv[1];
  nazwa_config = std::string(argv[2]);

  _macro(nazwa_program_dzialan);

  nazwa_plik_wykonawczy = nazwa_program_dzialan + ".tmp";

  std::cout << nazwa_plik_wykonawczy << std::endl;

  if (!ReadFile(nazwa_config.c_str(), Config)) return 1;

  ifstream process_instruction(nazwa_plik_wykonawczy);
  if (!process_instruction.is_open()) {
        cerr << "Error opening the file!";
        return 1;
    }
  std::string command;

  std::shared_ptr<LibInterface> Move (new LibInterface());
  std::shared_ptr<LibInterface> Rotate (new LibInterface());
  std::shared_ptr<LibInterface> Pause (new LibInterface());
  std::shared_ptr<LibInterface> Set (new LibInterface());

  Move->load_lib("Move");
  Rotate->load_lib("Rotate");
  Pause->load_lib("Pause");
  Set->load_lib("Set");

  Set4LibInterfaces interfaces;
  interfaces["Move"] = Move;
  interfaces["Rotate"] = Rotate;
  interfaces["Pause"] = Pause;
  interfaces["Set"] = Set;

  while (true) {
    
    if(! (process_instruction >> command)){
      break;
    }

    if (interfaces.find(command) == interfaces.end()) {
      std::cerr << "!!! Nieznane polecenie: " << command << std::endl;
      return false;
    }
    
    AbstractInterp4Command *pCmd = interfaces[command].get()->get_cmd();
    if (!pCmd) {
      std::cerr << "!!! Blad tworzenia polecenia: " << command << std::endl;
      return false;
    }

    if (!pCmd->ReadParams(process_instruction)) {
      std::cerr << "!!! Blad wczytywania parametrow polecenia: " << command << std::endl;
      return false;
    }

    pCmd->PrintCmd();

    delete pCmd;
  }

  return 0;
}
