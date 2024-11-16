#include "ProgramInterpreter.hh"

ProgramInterpreter::ProgramInterpreter()
{
     _Chann2Serv.open_connection();
     _Chann2Serv.send("Clear \n");
}

ProgramInterpreter::~ProgramInterpreter()
{
     _Chann2Serv.send("Close \n");
}

bool ProgramInterpreter::Read_XML_Config(const char *FileName_Conf, Configuration &rConfig)
{

     try
     {
          xercesc::XMLPlatformUtils::Initialize();
     }
     catch (const xercesc::XMLException &toCatch)
     {
          char *message = xercesc::XMLString::transcode(toCatch.getMessage());
          std::cerr << "Error during initialization! :\n";
          std::cerr << "Exception message is: \n"
                    << message << "\n";
          xercesc::XMLString::release(&message);
          return 1;
     }

     xercesc::SAX2XMLReader *pParser = xercesc::XMLReaderFactory::createXMLReader();

     pParser->setFeature(xercesc::XMLUni::fgSAX2CoreNameSpaces, true);
     pParser->setFeature(xercesc::XMLUni::fgSAX2CoreValidation, true);
     pParser->setFeature(xercesc::XMLUni::fgXercesDynamic, false);
     pParser->setFeature(xercesc::XMLUni::fgXercesSchema, true);
     pParser->setFeature(xercesc::XMLUni::fgXercesSchemaFullChecking, true);

     pParser->setFeature(xercesc::XMLUni::fgXercesValidationErrorAsFatal, true);

     xercesc::DefaultHandler *pHandler = new XMLInterp4Config(rConfig);
     pParser->setContentHandler(pHandler);
     pParser->setErrorHandler(pHandler);

     try
     {

          if (!pParser->loadGrammar("config/config.xsd",
                                    xercesc::Grammar::SchemaGrammarType, true))
          {
               std::cerr << "!!! Plik grammar/actions.xsd, '" << std::endl
                         << "!!! ktory zawiera opis gramatyki, nie moze zostac wczytany."
                         << std::endl;
               return false;
          }

          pParser->setFeature(xercesc::XMLUni::fgXercesUseCachedGrammarInParse, true);
          pParser->parse(FileName_Conf);

          XMLInterp4Config *pConfigHandler = dynamic_cast<XMLInterp4Config *>(pHandler);
          if (pConfigHandler)
          {
               // Uzyskanie dostępu do zmiennych
               Configuration &config = pConfigHandler->GetConfig();

               while (config.sizeLib() > 0)
               {
                    std::cout << "Dodano nową biblotekę:\t";
                    (*this)._LibManager.load_library(config.popLib());
               }

               while (config.sizeObj() > 0)
               {
                    std::shared_ptr<AbstractMobileObj> obj = config.popObject();
                    (*this)._Scn.AddMobileObj(obj);
                    std::string outString = "AddObj ";
                    outString += obj.get()->GetParams();
                    outString += " \n";
                    (*this)._Chann2Serv.send(outString.c_str());
                    std::cout << "Dodano nowy obiekt:\n" << outString << std::endl;
               }
          }
     }
     catch (const xercesc::XMLException &Exception)
     {
          char *sMessage = xercesc::XMLString::transcode(Exception.getMessage());
          std::cerr << "Informacja o wyjatku: \n"
                    << "   " << sMessage << "\n";
          xercesc::XMLString::release(&sMessage);
          return false;
     }
     catch (const xercesc::SAXParseException &Exception)
     {
          char *sMessage = xercesc::XMLString::transcode(Exception.getMessage());
          char *sSystemId = xercesc::XMLString::transcode(Exception.getSystemId());

          std::cerr << "Blad! " << std::endl
                    << "    Plik:  " << sSystemId << std::endl
                    << "   Linia: " << Exception.getLineNumber() << std::endl
                    << " Kolumna: " << Exception.getColumnNumber() << std::endl
                    << " Informacja: " << sMessage
                    << std::endl;

          xercesc::XMLString::release(&sMessage);
          xercesc::XMLString::release(&sSystemId);
          return false;
     }
     catch (...)
     {
          std::cout << "Zgloszony zostal nieoczekiwany wyjatek!\n";
          return false;
     }

     delete pParser;
     delete pHandler;
     return true;
}

bool ProgramInterpreter::ExecProgram(const char *FileName_Prog)
{

     std::ifstream process_instruction(FileName_Prog);
  if (!process_instruction.is_open()) {
        cerr << "Error opening the file!";
        return 1;
    }
  std::string command;

  while (true) {
    
    if(! (process_instruction >> command)){
      break;
    }

    if(!(*this)._LibManager.print_command(command, process_instruction)){
      return false;
    }

  }
return true;
}