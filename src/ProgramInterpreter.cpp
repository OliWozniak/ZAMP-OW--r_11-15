#include "ProgramInterpreter.hh"

ProgramInterpreter::ProgramInterpreter()
{
     _Chann2Serv.open_connection();
     _Chann2Serv.send("Clear \n");
}

ProgramInterpreter::~ProgramInterpreter(){
     _Chann2Serv.send("Close \n");
}

void ProgramInterpreter::Execute_commands(std::list<std::shared_ptr<AbstractInterp4Command>>& tasks){
 std::list<std::thread> threads;

    // Uruchamianie wątków
    for (std::shared_ptr<AbstractInterp4Command>& task : tasks) {
        threads.emplace_back([this, task]() {
            task->ExecCmd(_Scn, _Chann2Serv);
        });
    }

    // Zamknięcie wątków
    for (std::thread& t : threads) {
        if (t.joinable()) {
            t.join();
        }
    }   
}


bool ProgramInterpreter::Read_XML_Config(const char *FileName_Conf)
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
     Configuration rConfig;

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

          std::cout << "Liczba wgranych obiektów:\t" << rConfig.sizeObj() << std::endl;

          // XMLInterp4Config *pConfigHandler = dynamic_cast<XMLInterp4Config *>(pHandler);
          // Uzyskanie dostępu do zmiennych
          while (rConfig.sizeLib() > 0)
          {
               std::cout << "Dodano nową biblotekę:\t";
               (*this)._LibManager.load_library(rConfig.popLib());
          }

          while (rConfig.sizeObj() > 0)
          {
               std::shared_ptr<AbstractMobileObj> obj = rConfig.popObject();
               (*this)._Scn.AddMobileObj(obj);
               std::string outString = "AddObj ";
               outString += obj.get()->GetParams();
               outString += " \n";
               (*this)._Chann2Serv.send(outString.c_str());
               std::cout << "Dodano nowy obiekt:\n"
                         << outString << std::endl;
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
     if (!process_instruction.is_open())
     {
          cerr << "Error opening the file!";
          return 1;
     }

     std::string command;
     std::list<std::shared_ptr<AbstractInterp4Command>> tasks;
     bool is_parallel=false;

     while (true)
     {

          std::cout << "Nowa komenda:\t";

          if (!(process_instruction >> command))
          {
               break;
          }

          std::cout << "Nowa komenda:\t" << command << std::endl;

          if (command == "Begin_Parallel_Actions")
          {
               std::cout << "Akcja równoległa\n";
               is_parallel=true;
          }
          
          if (command == "End_Parallel_Actions")
          {
               is_parallel=false;
               std::cout << "Koniec równooległości\n";
               (*this).Execute_commands(tasks);
          }

          if ((*this)._LibManager.check_library(command))
          {
               if (is_parallel)
               {
                    std::cout << "Dodano nowe zadanie\n";
                   tasks.push_back((*this)._LibManager.make_cmd(command, process_instruction)); 
               } else{
                    std::shared_ptr<AbstractInterp4Command> tmp_p= (*this)._LibManager.make_cmd(command, process_instruction);
                    tmp_p.get()->ExecCmd(_Scn, _Chann2Serv);
               }
               
          } 
          // else{return false;}
          
          

          // if (!(*this)._LibManager.print_command(command, process_instruction)){return false;}
     }
     return true;
}