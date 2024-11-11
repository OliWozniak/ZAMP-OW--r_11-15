#ifndef PROGRAMINTERPRETER_HH
#define PROGRAMINTERPRETER_HH

#include "Scene.hh"
#include "Set4LibInterfaces.hh"
#include "ComChannel.hh"
#include "Configuration.hh"
#include "xmlinterp.hh"


#include <xercesc/sax2/SAX2XMLReader.hpp>
#include <xercesc/sax2/XMLReaderFactory.hpp>
#include <xercesc/sax2/DefaultHandler.hpp>
#include <xercesc/util/XMLString.hpp>

class ProgramInterpreter
{
private:
    Scene _Scn;
    Set4LibInterfaces _LibManager;
    ComChannel _Chann2Serv;

public:
    ProgramInterpreter(/* args */);
    ~ProgramInterpreter();

    bool Read_XML_Config(const char* FileName_Conf);
    bool ExecProgram(const char* FileName_Prog);
};


#endif